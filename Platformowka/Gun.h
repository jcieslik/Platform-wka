#pragma once
#include "Pickup.h"
class Gun : public Pickup
{
	static const std::string texture_filename;
public:
	Gun(int x, int y)
	{
		size = sf::Vector2f(24.f, 24.f);
		pickup_sprite.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
		this->x = x;
		this->y = y;
		pickup_sprite.setPosition(sf::Vector2f(32.f * y + 4.f, 32.f * x + 4.f));
	}
	void player_pickup(Player& player);
};

