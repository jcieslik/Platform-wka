#include <iostream>
#include <fstream>
#include <string>
#include "Game_objects.h"
#include "Upgrade.h"
#include "Simple_tile.h"
#include "Skeleton.h"
#include "Cowboy.h"
#include "Finish.h"
#include "Spikes.h"
#include "Gun.h"
#include "Coin.h"
#include "Rat.h"

using namespace std;

pair<int, int> Game_objects::load_map(std::string& filename)
{
	int height = 0;
	int width = 0;
	ifstream file;
	file.open(filename);	
	if (file.is_open())
	{
		string line; 
		getline(file, line);
		map.push_back(line);
		width = static_cast<int>(line.size()); // sprawdzenie szerokosc poziomu (kazda linia musi miec taka sama)
		for (string line; getline(file, line);)
		{
			if (line.size() != width)
				throw exception("Niepoprawny plik");
			map.push_back(line);
		}
		height = static_cast<int>(map.size());  // dlugosc wektora to wysokosc
		file.close();
	}
	if(!load_objects(width, height))   // proba wczytania obiektow z pliku
		throw exception("Niepoprawny plik");
	return make_pair(width, height);   // zwracamy wymiary poziomu
}

bool Game_objects::load_objects(int width, int height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (!create_object(map[i][j], i, j))  // jezeli wystapil blad pliku z teksturami przy tworzeniu obiektu, zwroc falsz
				return false;
		}
	return true;
}

bool Game_objects::create_object(char type, int x, int y)
{
	// tworzenie obiektow moze zwrocic wyjatek przy wczytywaniu tekstur
	try {
		if (type == 'G')
			tilemap.insert(make_pair(make_pair(x, y), make_unique<Simple_tile>(x, y)));
		else if (type == 'S')
			tilemap.insert(make_pair(make_pair(x, y), make_unique<Spikes>(x, y)));
		else if (type == 'F')
			tilemap.insert(make_pair(make_pair(x, y), make_unique<Finish>(x, y)));
		else if (type == 'C')
			pickups.insert(make_pair(make_pair(x, y), make_unique<Coin>(x, y)));
		else if (type == 'M')
			pickups.insert(make_pair(make_pair(x, y), make_unique<Gun>(x, y)));
		else if (type == 'J')
			pickups.insert(make_pair(make_pair(x, y), make_unique<Upgrade>(x, y)));
		else if (type == 'R')
			enemies.push_back(make_unique<Rat>(x, y));
		else if (type == 'W')
			enemies.push_back(make_unique<Cowboy>(x, y));
		else if (type == 'K')
			enemies.push_back(make_unique<Skeleton>(x, y));
		return true;
	}
	catch (exception e)
	{
		return false;
	}
}