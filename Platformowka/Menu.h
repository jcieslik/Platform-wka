#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <list>
#include "Leaderboard.h"
#include "Game.h"

class Menu
{
private:
	sf::RenderWindow window;
	sf::Sprite background;
	sf::Font menu_font;
	sf::Music music;
	Leaderboard menu_leaderboard;
	std::string path = "levels/";
	std::vector<std::string> level_filenames;
	std::vector<menu_options> options = { menu_options::play, menu_options::leaderboards, menu_options::end };
public:
	Menu()
	{
		background.setTexture(Resource_holder::get_resources().get_texture("images/Menu_background.jpg"));
		background.setTextureRect(sf::IntRect(0, 0, 800, 600));
		window.create(sf::VideoMode(800, 600), "Platformowka", sf::Style::Close);
		music.openFromFile("Muzyka.ogg");
		music.setVolume(10.f);
		window.setFramerateLimit(60);
		menu_font.loadFromFile("VCR.ttf");
	}
	void set_options(sf::Text options[]);
	void main_menu();
	void display_leaderboards();
	bool start_game();
	void end_screen();
	void end_screen_with_leaderboards(int player_points);
	void load_levels();
};