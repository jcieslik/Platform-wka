#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"
#include "Resource_holder.h"

class Pickup 
{
protected:
	sf::Sprite pickup_sprite;
	sf::Vector2f size;
	int x;
	int y;
public:
	Pickup()
	{
		x = 0;
		y = 0;
	}
	void draw(sf::RenderTarget& unit, sf::RenderStates state) const
	{
		unit.draw(pickup_sprite);
	}
	sf::Sprite get_pickup()
	{
		return pickup_sprite;
	}
	void set_position(sf::Vector2f new_position)
	{
		pickup_sprite.setPosition(new_position);
	}
	bool collision_check(Unit* object);
	virtual void player_pickup(Player& player) = 0;
};
