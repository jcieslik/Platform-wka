#pragma once
#include <SFML/Graphics.hpp>
#include "Resource_holder.h"

class Bullet
{
	sf::Sprite bullet_sprite;
	sf::Vector2f velocity;
	float distance_flown;
	float speed;
public:
	Bullet()
	{
		distance_flown = 0.f;
		speed = 250.f;
		bullet_sprite.setTexture(Resource_holder::get_resources().get_texture("images/Bullet.png"));
	}
	void set_position(sf::Vector2f new_position)
	{
		bullet_sprite.setPosition(new_position);
	}
	void change_direction()
	{
		speed = -speed;
	}
	sf::Sprite get_bullet()
	{
		return bullet_sprite;
	}
	sf::Vector2f get_position()
	{
		return bullet_sprite.getPosition();
	}
	float get_flown_distance()
	{
		return distance_flown;
	}
	void move_bullet(float deltaTime);
};

