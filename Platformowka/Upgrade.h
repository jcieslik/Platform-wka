#pragma once
#include "Pickup.h"
class Upgrade : public Pickup
{
	float speed_increase;
	float jump_increase;
	static const std::string texture_filename;
public:
	Upgrade(int x, int y)
	{
		speed_increase = 20.f;
		jump_increase = 5.f;
		size = sf::Vector2f(24.f, 24.f);
		pickup_sprite.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
		this->x = x;
		this->y = y;
		pickup_sprite.setPosition(sf::Vector2f(32.f * y + 4.f, 32.f * x + 4.f));
	}
	void player_pickup(Player& player);
};
