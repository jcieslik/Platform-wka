#pragma once
#include "Unit.h"
#include "Player.h"

class Cowboy : public Unit
{
	sf::Clock shoot_timer;
	std::vector<std::unique_ptr<Bullet>> enemy_bullets;
	static const std::string texture_filename;
public:
	Cowboy(int x, int y)
	{
		points_worth = 500;
	    unit_shape.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
	    size = sf::Vector2f(24.f, 24.f);
		jump_height = 120.f;
		can_jump = false;
		this->x = x;
		this->y = y;
		unit_shape.setPosition(sf::Vector2f(32.f * this->y, 32.f * this->x));
	}
	void allow_jump(bool allow) 
	{
		can_jump = allow;
	}
	std::vector<std::unique_ptr<Bullet>>* get_bullets()
	{
		return &enemy_bullets;
	}
	void update_position(float deltaTime, Unit* player);
	void update_position(float deltaTime) {};
	void collision_up(Unit* object);
	void collision_down(Unit* object);
	void collision_left(Unit* object);
	void collision_right(Unit* object);
	void change_direction(direction dir);
	void shoot_bullet();
};

