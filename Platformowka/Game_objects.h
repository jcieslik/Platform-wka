#pragma once
#include "Unit.h"
#include "Tile.h"
#include "Pickup.h"
#include "Resource_holder.h"

class Game_objects
{
	std::map<std::pair<int, int>, std::unique_ptr<Tile>> tilemap;
	std::map<std::pair<int, int>, std::unique_ptr<Pickup>> pickups;
	std::vector<std::unique_ptr<Unit>> enemies;
	std::vector<std::string> map;
public:
	friend class Level;
	std::pair<int, int> load_map(std::string& filename);
	bool load_objects(int width, int height);
	bool create_object(char type, int x, int y);
};

