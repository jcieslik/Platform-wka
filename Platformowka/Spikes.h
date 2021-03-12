#pragma once
#include "Tile.h"

class Spikes : public Tile
{
	static const std::string texture_filename;
public:
	Spikes(int x, int y)
	{
		size = sf::Vector2f(32.f, 32.f);
		tile_sprite.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
		this->x = x;
		this->y = y;
		tile_sprite.setPosition(sf::Vector2f(32.f * y, 32.f * x));
	}
	void collision_up(Unit* object);
	void collision_down(Unit* object);
	void collision_left(Unit* object);
	void collision_right(Unit* object);
};
