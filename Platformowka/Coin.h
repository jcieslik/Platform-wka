#pragma once
#include "Pickup.h"
#include "Player.h"

class Coin : public Pickup
{
	int value;
	static const std::string texture_filename;
public:
	Coin(int x, int y)
	{
		value = 100;
		size = sf::Vector2f(24.f, 24.f);
		pickup_sprite.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
		this->x = x;
		this->y = y;
		pickup_sprite.setPosition(sf::Vector2f(32.f * y + 4.f, 32.f * x + 4.f));
	}
	void player_pickup(Player& player);
};

