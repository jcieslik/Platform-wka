#include <iostream>
#include "Level.h"
#include "Skeleton.h"

using namespace std;

bool Level::load_player()
{
	// znajdywanie P w wektorze stringow (pozycja gracza)
	vector<string>::iterator iter;
	int i = 0;
	size_t found = 0;
	for (iter = objects.map.begin(); iter != objects.map.end(); ++iter)
	{
		found = (*iter).find('P');
		if (found != string::npos)
		{
			player_spawn = sf::Vector2f(found * 32.f, i * 32.f);
			return true;
		}
		i++;
	}
	return false; // jezeli nie ustawiono gracza, zwroc falsz
}

bool Level::load_tilemap(string& filename)
{
	// proba wczytania mapy
	try
	{
		pair<int, int> width_and_height = objects.load_map(filename);
		width = width_and_height.first;
		height = width_and_height.second;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return false;
	}

	return true;
}

void Level::draw_level(sf::RenderWindow& window)
{
	// rysowanie obiektow gry
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (objects.tilemap[make_pair(i, j)] != nullptr)
				window.draw(objects.tilemap[make_pair(i, j)]->get_tile());
			else if (objects.pickups[make_pair(i, j)] != nullptr)
				window.draw(objects.pickups[make_pair(i, j)]->get_pickup());
		}
	vector<unique_ptr<Unit>>::const_iterator unit_iter;
	for (unit_iter = objects.enemies.begin(); unit_iter != objects.enemies.end(); ++unit_iter)
		window.draw((*unit_iter)->get_unit());
}

void Level::check_collision_horizontal(Unit* object)
{
	// sprawdzanie kolizji poziomo
	direction collision_side = direction::no_direction;
	int x = int(object->get_position().x / 32.f);
	int y = int(object->get_position().y / 32.f);
	for (int i = y - 1; i <= y + 1; i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (objects.tilemap[make_pair(i, j)] != nullptr) // jezeli w danym miejscu jest blok
			{
				collision_side = objects.tilemap[make_pair(i, j)]->collision_check(object); //sprawdz, z ktorej strony jest kolizja
				if (collision_side == direction::left)  // jezeli z lewej, obsluz kolizje z lewej
					objects.tilemap[make_pair(i, j)]->collision_left(object);
				else if (collision_side == direction::right)  // jezeli z prawej, obsluz kolizje z prawej
					objects.tilemap[make_pair(i, j)]->collision_right(object);
			}
		}
	}
	for (auto const& elem : objects.enemies)
	{
		if (elem.get() != object)
		{
			collision_side = elem->collision_check(object);
			if (collision_side == direction::left)
				elem->collision_left(object);
			else if (collision_side == direction::right)
				elem->collision_right(object);
		}
	}
}

void Level::check_collision_vertical(Unit* object)
{
	// sprawdzanie kolizji w pionie
	direction collision_side = direction::no_direction;
	int x = int(object->get_position().x / 32.f);
	int y = int(object->get_position().y / 32.f);

	for (int i = y - 1; i <= y + 1; i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (objects.tilemap[make_pair(i, j)] != nullptr)
			{
				collision_side = objects.tilemap[make_pair(i, j)]->collision_check(object);
				if (collision_side == direction::up)
					objects.tilemap[make_pair(i, j)]->collision_up(object);
				else if (collision_side == direction::down)
					objects.tilemap[make_pair(i, j)]->collision_down(object);
			}
		}
	}
	for (auto const& elem : objects.enemies)
	{
		if (elem.get() != object)
		{
			collision_side = elem->collision_check(object);
			if (collision_side == direction::up)
				elem->collision_up(object);
			else if (collision_side == direction::down)
				elem->collision_down(object);
		}
	}
}

void Level::check_for_fall(Unit* object)
{
	int x = int(object->get_position().x / 32.f);
	int y = int(object->get_position().y / 32.f);
	if (dynamic_cast<Skeleton*>(object) && object->get_velocity().y == 0.f) // jezeli to szkielet i nie spada w dol (zacinalby sie w scianie)
	{
		if (object->get_direction() == direction::right) // jezeli idzie w prawo
		{
			if (objects.tilemap[make_pair(y, x + 1)] == nullptr && objects.tilemap[make_pair(y + 1, x + 1)] == nullptr
				&& objects.tilemap[make_pair(y + 1, x)] != nullptr && objects.tilemap[make_pair(y + 2, x + 1)] == nullptr
				&& objects.tilemap[make_pair(y + 2, x + 2)] == nullptr)
				object->change_direction_left(); // jezeli jest pod nim blok, a odpowiednich blokow po prawej nie ma, zawroc (jest przepasc)
			else if (objects.tilemap[make_pair(y - 1, x + 1)] == nullptr && objects.tilemap[make_pair(y - 1, x + 2)] == nullptr
				&& objects.tilemap[make_pair(y, x + 2)] != nullptr && object->check_can_jump()) 
				object->jump(object->get_jump_height()); // jezeli jest przed nim przeszkoda i nie ma nad nim sufitu, przeskocz przeszkode 
		}
		else // gdy idzie w lewo, sprawdza odpowiednie bloki po lewej
		{
			if (objects.tilemap[make_pair(y, x)] == nullptr && objects.tilemap[make_pair(y + 1, x)] == nullptr
				&& objects.tilemap[make_pair(y + 1, x + 1)] != nullptr && objects.tilemap[make_pair(y + 2, x)] == nullptr
				&& objects.tilemap[make_pair(y + 2, x - 1)] == nullptr)
					object->change_direction_right();
			else if (objects.tilemap[make_pair(y - 1, x)] == nullptr && objects.tilemap[make_pair(y - 1, x - 1)] == nullptr
				&& objects.tilemap[make_pair(y, x - 1)] != nullptr && object->check_can_jump())
				object->jump(object->get_jump_height());
		}
	}
	else
	{
		if (object->get_velocity().y == 0.f)
		{
			if (object->get_direction() == direction::right)
			{
				if (objects.tilemap[make_pair(y, x + 1)] == nullptr && objects.tilemap[make_pair(y + 1, x + 1)] == nullptr
					&& objects.tilemap[make_pair(y + 1, x)] != nullptr) // sprawdz czy jest przepasc, jezeli tak to zmien kierunek
					object->change_direction_left();
			}
			else if (objects.tilemap[make_pair(y, x)] == nullptr && objects.tilemap[make_pair(y + 1, x)] == nullptr
				&& objects.tilemap[make_pair(y + 1, x + 1)] != nullptr)
				object->change_direction_right(); // to samo tylko w lewo
		}
	}
}

void Level::check_for_pickups(Player& player)
{
	int x = int(player.get_position().x / 32.f);
	int y = int(player.get_position().y / 32.f);
	for (int i = y - 1; i <= y + 1; i++)
	{
		for (int j = x - 1; j <= x + 1; j++)
		{
			if (objects.pickups[make_pair(i, j)] != nullptr && objects.pickups[make_pair(i, j)]->collision_check(&player))
			{
				objects.pickups[make_pair(i, j)]->player_pickup(player);
				objects.pickups[make_pair(i, j)].reset(); //usuwanie przedmiotu po zebraniu
			}
		}
	}
}
