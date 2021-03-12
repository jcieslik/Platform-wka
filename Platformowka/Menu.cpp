#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>
#include <string>
#include "Game.h"
#include "Menu.h"

using namespace std;

void Menu::set_options(sf::Text options[])
{
    options[0].setString("Rozpocznij gre");
    options[1].setString("Najlepsze wyniki");
    options[2].setString("Zakoncz gre");
    for (int i = 0; i < 3; i++)
    {
        options[i].setCharacterSize(30);
        options[i].setFont(menu_font);
        options[i].setPosition((window.getSize().x - options[i].getGlobalBounds().width) / 2, 100.f * (i + 1));
    }
    options[0].setFillColor(sf::Color::Yellow);
}

void Menu::main_menu()
{
    load_levels();
    sf::Text options_text[3];
    set_options(options_text);
    menu_leaderboard.load_leaderboards();
    vector<menu_options>::const_iterator iter = options.begin();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up && iter != options.begin())
                {
                    options_text[iter - options.begin()].setFillColor(sf::Color::White);
                    iter--;
                    options_text[iter - options.begin()].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Down && next(iter, 1) != options.end())
                {
                    options_text[iter - options.begin()].setFillColor(sf::Color::White);
                    iter++;
                    options_text[iter - options.begin()].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    if ((*iter) == menu_options::play)
                    {
                        if (!start_game())
                            return;
                    }
                    else if ((*iter) == menu_options::leaderboards)
                        display_leaderboards();
                    else if ((*iter) == menu_options::end)
                    {
                        menu_leaderboard.save_leaderboards();
                        return;
                    }
                }

            }
        }
        window.clear();
        window.setView(window.getDefaultView());
        window.draw(background);
        for (int i = 0; i < 3; i++)
            window.draw(options_text[i]);
        window.display();
    }
}

void Menu::display_leaderboards()
{
    vector<sf::Text> best_players;
    sf::Text player("", menu_font, 25);
    sf::Text continue_text("Kontynuuj", menu_font, 30);
    continue_text.setPosition((window.getSize().x - continue_text.getGlobalBounds().width) / 2, 500.f);
    continue_text.setFillColor(sf::Color::Yellow);
    for (int i = 0; i < menu_leaderboard.get_entries().size(); i++)
    {
        player.setString((menu_leaderboard.get_entries()[i].second) + "  " + to_string(menu_leaderboard.get_entries()[i].first));
        player.setPosition((window.getSize().x - player.getGlobalBounds().width) / 2, 40.f * i + 40.f);
        best_players.push_back(player);
    }
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
                    return;
            }
        }
        window.clear();
        window.setView(window.getDefaultView());
        for (int i = 0; i < best_players.size(); i++)
            window.draw(best_players[i]);
        window.draw(continue_text);
        window.display();
    }
}

bool Menu::start_game()
{
    // przechodzenie przez kolejne poziomy i sprawdzanie stanu gry
    Game main_game;
    vector<string>::iterator iter;
	state_of_game state;
    music.play();
    music.setLoop(true);
	for (iter = level_filenames.begin(); iter != level_filenames.end(); ++iter)
	{
		state = main_game.play_level(window, (*iter));
        if (state == state_of_game::lost)
        {
            if (main_game.get_player_points() > 0)
            {
                if (menu_leaderboard.get_entries().back().first < main_game.get_player_points() || menu_leaderboard.get_entries().size() < 10)
                    end_screen_with_leaderboards(main_game.get_player_points());  // jezeli mozna wpisac nowy najlepszy wynik, to wywolaj metode z wpisem
                else end_screen();  // w przeciwnym razie zwykly koniec gry
            }
            else end_screen();
            return true;
        }
        if (state == state_of_game::file_error || state == state_of_game::error)
            return false;
	}
    if (menu_leaderboard.get_entries().back().first < main_game.get_player_points() || menu_leaderboard.get_entries().size() < 10)
        end_screen_with_leaderboards(main_game.get_player_points());
    else end_screen();
    return true;
}

void Menu::end_screen()
{
    music.stop();
    sf::Text end_text("Koniec gry", menu_font, 36);
    sf::Text continue_text("Kontynuuj", menu_font, 30);
    continue_text.setPosition((window.getSize().x - continue_text.getGlobalBounds().width) / 2, 500.f);
    continue_text.setFillColor(sf::Color::Yellow);
    end_text.setPosition((window.getSize().x - end_text.getGlobalBounds().width) / 2, 100.f);
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
                    return;
            }
        }
        window.clear();
        window.setView(window.getDefaultView());
        window.draw(end_text);
        window.draw(continue_text);
        window.display();
    }
}

void Menu::end_screen_with_leaderboards(int player_points)
{
    music.stop();
    sf::String player_input;
    sf::Text player_text("", menu_font, 24);
    sf::Text end_text("Koniec gry", menu_font, 36);
    sf::Text Info("Gratulacje! Wpisz pseudonim (max 12 znakow):", menu_font, 20);
    sf::Text continue_text("Kontynuuj", menu_font, 30);
    continue_text.setPosition((window.getSize().x - continue_text.getGlobalBounds().width) / 2, 500.f);
    continue_text.setFillColor(sf::Color::Yellow);
    end_text.setPosition((window.getSize().x - end_text.getGlobalBounds().width) / 2, 100.f);
    Info.setPosition((window.getSize().x - Info.getGlobalBounds().width) / 2, 200.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter && player_input.getSize())
                {
                    menu_leaderboard.add_to_leaderboard(player_points, player_input);
                    return;
                }
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (menu_leaderboard.get_entries().back().first < player_points || menu_leaderboard.get_entries().size() < 10)
                {
                    if (event.text.unicode == 8)
                    {
                        if (player_input.getSize())
                            player_input.erase(player_input.getSize() - 1, player_input.getSize());
                    }
                    else if (player_input.getSize() < 12 && event.text.unicode != 13) // 13 to kod entera, nie przyjmuje go (sluzy do akceptacji)
                        player_input += static_cast<char>(event.text.unicode);
                }
                player_text.setString(player_input);
                player_text.setPosition((window.getSize().x - player_text.getGlobalBounds().width) / 2, 300.f);
            }
        }
        window.clear();
        window.setView(window.getDefaultView());
        window.draw(player_text);
        window.draw(end_text);
        window.draw(Info);
        window.draw(continue_text);
        window.display();
    }
}

void Menu::load_levels()
{
    // wczytywanie plikow z folderu levels
    string filename;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        filename = entry.path().u8string();
        level_filenames.push_back(filename);
    }
}
