#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Level.h"

class Game
{
	sf::Sprite hearts[3];
	sf::Sprite background;
	sf::Sprite ammo;
	sf::View player_view;
	sf::View user_interface;
	sf::Font game_font;
	sf::Text points_text;
	sf::Text clock_text;
	sf::Text ammo_text;
	sf::Vector2f view_size;
	Player player;
	Level* level;
	float level_time;
	float deltaTime;
	int current_level;
public:	
	Game()
	{
		level_time = 300.f;
		view_size = sf::Vector2f(400.f, 300.f);
		Resource_holder::get_resources().get_texture("images/Background.png").setRepeated(true);
		background.setTexture(Resource_holder::get_resources().get_texture("images/Background.png"));
		ammo.setTexture(Resource_holder::get_resources().get_texture("images/Bullet.png"));
		deltaTime = 0.f;
		current_level = 0;
		player_view.setSize(view_size);
		user_interface.setSize(view_size);
		user_interface.setCenter(sf::Vector2f(view_size.x/2, view_size.y/2));
		this->set_view();
		game_font.loadFromFile("VCR.ttf");
		set_interface();
	}
	int get_player_points()
	{
		return player.get_points();
	}
	void set_level(Level* new_level)
	{
		level = new_level;
		background.setTextureRect(sf::IntRect(0, 0, level->get_width() * 32 + 800, level->get_height() * 32 + 600));
	}
	void reset_level_timer()
	{
		level_time = 300.f;
	}
	void set_interface();
	void draw_interface(sf::RenderWindow& window);
	void level_bounds(Unit* object);
	void set_playerpos();
	void draw_bullets(sf::RenderWindow& window);
	void draw_enemy_bullets(sf::RenderWindow& window);
	void move_enemies();
	void update_player();
	void update_bullets();
	void update_enemy_bullets();
	bool check_if_hit_player(Bullet* bullet);
	bool check_if_hit_enemy(Bullet* bullet);
	state_of_game play_level(sf::RenderWindow& window, std::string& level_filename);
	void set_view();
	bool pause_game(sf::RenderWindow& window);
};

