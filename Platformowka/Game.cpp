#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "Game.h"
#include "Unit.h"
#include "Player.h"
#include "Level.h"

using namespace std;

state_of_game Game::play_level(sf::RenderWindow& window, std::string& level_filename)
{
    window.setFramerateLimit(100);
    Level level;
    if (!level.load_tilemap(level_filename) || !level.load_player())
        return state_of_game::file_error;

    //ustaw poziom, pozycje gracza i zegary
    set_level(&level);
    set_playerpos();
    player.reset_spawn_timer();
    reset_level_timer();
	sf::Clock clock;
  
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        if (deltaTime > 1.0f / 30.0f)
            deltaTime = 1.0f / 30.0f;
        level_time -= deltaTime;  // zmniejsz pozostaly czas

        sf::Event event;
        while (window.pollEvent(event))
		{
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    if (!pause_game(window))
                        return state_of_game::lost;
            }
        }
        update_player(); 
        move_enemies();
        update_bullets();
        update_enemy_bullets();

		window.clear();
        window.setView(player_view);
        window.draw(background); 
        level.draw_level(window);
        draw_bullets(window);
        draw_enemy_bullets(window);
        window.draw(player);
        window.setView(user_interface);
        draw_interface(window);
        window.display();

        if (player.get_completed())
        {
            player.set_completed(false);
            player.add_points(500 + static_cast<int>(level_time) * 5);
            return state_of_game::completed_level;
        }
        else if (player.get_lives() == 0 || static_cast<int>(level_time) <= 0)
            return state_of_game::lost;
    } 
    return state_of_game::error;
}

void Game::set_view()
{
    // ustawianie kamery podazajacej za graczem
    {
        if (player.get_position().x + player.get_size().x / 2 < view_size.x/2 && player.get_position().y + player.get_size().y / 2 < view_size.y / 2)
        {
            player_view.setCenter(sf::Vector2f(view_size.x / 2, view_size.y / 2));
            return;
        }
        else if (player.get_position().x + player.get_size().x / 2 < view_size.x / 2)
        {
            player_view.setCenter(sf::Vector2f(view_size.x / 2, player.get_position().y + player.get_size().y / 2));
            return;
        }
        else if (player.get_position().y + player.get_size().y / 2 < view_size.y / 2)
        {
            player_view.setCenter(sf::Vector2f(player.get_position().x + player.get_size().x/2, view_size.y / 2));
            return;
        }
        player_view.setCenter(player.get_position() + sf::Vector2f(player.get_size().x/2, player.get_size().y / 2));
    }
}

bool Game::pause_game(sf::RenderWindow& window)
{
    pause_options chosen = pause_options::continue_game;
    sf::Text continue_text("Kontynuuj", game_font, 30);
    sf::Text pause("Pauza", game_font, 40);
    sf::Text end("Zakoncz", game_font, 30);
    end.setPosition((window.getSize().x - end.getGlobalBounds().width) / 2, 300.f);
    continue_text.setPosition((window.getSize().x - continue_text.getGlobalBounds().width) / 2, 400.f);
    continue_text.setFillColor(sf::Color::Yellow);
    pause.setPosition((window.getSize().x - pause.getLocalBounds().width) / 2, 100.f);
    while (window.isOpen())
    {
       
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    if (chosen == pause_options::continue_game)
                        return true;
                    else return false;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    chosen = pause_options::continue_game;
                    continue_text.setFillColor(sf::Color::Yellow);
                    end.setFillColor(sf::Color::White);
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    chosen = pause_options::end;
                    end.setFillColor(sf::Color::Yellow);
                    continue_text.setFillColor(sf::Color::White);
                }
            }
        }
        
        window.clear();
        window.setView(window.getDefaultView());
        window.draw(pause);
        window.draw(continue_text);
        window.draw(end);
        window.display();
    }
    return true;
}

void Game::move_enemies()
{
    vector<unique_ptr<Unit>>::const_iterator unit_iter;
    for (unit_iter = level->get_enemies().begin(); unit_iter != level->get_enemies().end(); ++unit_iter)
    {
        (*unit_iter)->update_position(deltaTime, &player);
        level->check_collision_vertical((*unit_iter).get());
        level->check_collision_horizontal((*unit_iter).get());
        level->check_for_fall((*unit_iter).get());

        level_bounds((*unit_iter).get());
        if ((*unit_iter)->get_position().y > level->get_height() * 32.f + 200.f)
        {
            unit_iter = level->get_enemies().erase(unit_iter);
            if (unit_iter == level->get_enemies().end())  // jezeli przeciwnik wypadl poza poziom, usun go
                return;
        }
    }
}

void Game::update_player()
{
    if (player.get_position().y > level->get_height() * 32.f + 200.f) 
        player.damage(); // jezeli gracz wypadl poza poziom, traci zycie
    player.update_position(deltaTime);
    level->check_collision_horizontal(&player);
    level->check_collision_vertical(&player);
    level->check_for_pickups(player);   

    level_bounds(&player);
    set_view();
}

void Game::update_bullets()
{
    vector<unique_ptr<Bullet>>::const_iterator iter;
    for (iter = player.get_bullets()->begin(); iter != player.get_bullets()->end();)
    {
        if (check_if_hit_enemy((*iter).get()))
            iter = player.get_bullets()->erase(iter); // jezeli pocisk wykryl kolizje, usun go
        else ++iter;
    }
}

void Game::update_enemy_bullets()
{
    vector<unique_ptr<Unit>>::const_iterator iter;
    vector<unique_ptr<Bullet>>::const_iterator bullet_iter;
    for (iter = level->get_enemies().begin(); iter != level->get_enemies().end(); ++iter)
    {
        if ((*iter)->get_bullets() != nullptr)
        {
            for(bullet_iter = (*iter)->get_bullets()->begin(); bullet_iter != (*iter)->get_bullets()->end();)
            {
                if (check_if_hit_player((*bullet_iter).get()))
                    bullet_iter = (*iter)->get_bullets()->erase(bullet_iter);  // jezeli pocisk wykryl kolizje, usun go
                else ++bullet_iter;
            }
        }
    }
}

bool Game::check_if_hit_enemy(Bullet* bullet)
{
    // sprawdzanie kolizji pocisku gracza
    bullet->move_bullet(deltaTime);
    int x = int(bullet->get_position().x / 32.f);
    int y = int(bullet->get_position().y / 32.f);
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (level->get_tilemap()[make_pair(i, j)] != nullptr)
                if (bullet->get_bullet().getGlobalBounds().intersects(level->get_tilemap()[make_pair(i, j)]->get_tile().getGlobalBounds()))
                    return true;
        }
    }
    vector<unique_ptr<Unit>>::const_iterator unit_iter = level->get_enemies().begin();
    while (unit_iter != level->get_enemies().end())
    {
        if (bullet->get_bullet().getGlobalBounds().intersects((*unit_iter)->get_unit().getGlobalBounds()))
        {
            player.add_points((*unit_iter)->get_points_worth());
            level->get_enemies().erase(unit_iter);
            return true;
        }
        ++unit_iter;
    }
    if (bullet->get_flown_distance() > 1000.f)
        return true;
    return false;
}

bool Game::check_if_hit_player(Bullet* bullet)
{
    // sprawdzanie kolizji pocisku przeciwnika 
    bullet->move_bullet(deltaTime);
    int x = int(bullet->get_position().x / 32.f);
    int y = int(bullet->get_position().y / 32.f);
    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            if (level->get_tilemap()[make_pair(i, j)] != nullptr)
                if (bullet->get_bullet().getGlobalBounds().intersects(level->get_tilemap()[make_pair(i, j)]->get_tile().getGlobalBounds()))
                    return true;
        }
    }
    if (bullet->get_bullet().getGlobalBounds().intersects(player.get_unit().getGlobalBounds()))
    {
        player.damage();
        return true;
    }
    else if (bullet->get_flown_distance() > 1000.f)
        return true;
    return false;
}

void Game::level_bounds(Unit* object)
{
    // nie pozwalanie na wychodzenie poza poziom z gory i z lewej
    if (object->get_position().x < 0.f)
    {
        object->set_position(sf::Vector2f(0.f, object->get_position().y));
        object->change_direction(direction::right);
    }
    if (object->get_position().y < 0.f)
    {
        object->set_position(sf::Vector2f(object->get_position().x, 0.f));
        object->set_velocity_y(10.f);
    }
}

void Game::set_playerpos()
{
    player.set_position(sf::Vector2f(level->get_player_spawn()));
    player.set_spawn_position(sf::Vector2f(level->get_player_spawn()));
}

void Game::draw_bullets(sf::RenderWindow& window)
{
    vector<unique_ptr<Bullet>>::const_iterator iter;
    for (iter = player.get_bullets()->begin(); iter != player.get_bullets()->end(); ++iter)
        window.draw((*iter)->get_bullet());
}

void Game::draw_enemy_bullets(sf::RenderWindow& window)
{
    vector<unique_ptr<Unit>>::const_iterator iter;
    vector<unique_ptr<Bullet>>::const_iterator bullet_iter;
    for (iter = level->get_enemies().begin(); iter != level->get_enemies().end(); ++iter)
    {
        if ((*iter)->get_bullets() != nullptr)
        {
            for (bullet_iter = (*iter)->get_bullets()->begin(); bullet_iter != (*iter)->get_bullets()->end(); ++bullet_iter)
                window.draw((*bullet_iter)->get_bullet());
        }
    }
}

void Game::draw_interface(sf::RenderWindow& window)
{
    for (int i = 0; i < player.get_lives(); i++)
        window.draw(hearts[i]);
    points_text.setString(to_string(player.get_points()));
    points_text.setPosition((view_size.x - points_text.getGlobalBounds().width) / 2, 6.f);
    clock_text.setString(to_string(static_cast<int>(level_time)));
    clock_text.setPosition((view_size.x - clock_text.getLocalBounds().width), 6.f);
    ammo_text.setString(to_string(player.get_bullets_amount()));
    ammo_text.setPosition((view_size.x - ammo_text.getLocalBounds().width), 280.f);
    window.draw(points_text);
    window.draw(clock_text);
    window.draw(ammo_text);
    window.draw(ammo);
}

void Game::set_interface()
{
    clock_text.setFont(game_font);
    clock_text.setString(to_string(static_cast<int>(level_time)));
    clock_text.setCharacterSize(15);
    clock_text.setOrigin(10.f, 0.f);
    points_text.setFont(game_font);
    points_text.setString(to_string(player.get_points()));
    points_text.setCharacterSize(15);
    ammo_text.setFont(game_font);
    ammo_text.setOrigin(10.f, 0.f);
    ammo_text.setString(to_string(player.get_bullets_amount()));
    ammo_text.setCharacterSize(15);
    ammo.setScale(1.5f, 1.5f);
    ammo.setPosition((view_size.x - 60.f), 284.f);
    for (int i = 0; i < 3; i++)
    {
        hearts[i].setTexture(Resource_holder::get_resources().get_texture("images/Heart.png"));
        hearts[i].setPosition(sf::Vector2f(25.f * i + 5.f, 5.f));
    }
}
