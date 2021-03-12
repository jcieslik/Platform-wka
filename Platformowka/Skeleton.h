#pragma once
#include "Unit.h"

class Skeleton : public Unit
{
	static const std::string texture_filename;
public:
	Skeleton(int x, int y)
	{
		points_worth = 200;
		jump_height = 40.f;
		speed = 125.f;
		size = sf::Vector2f(25.f, 25.f);
		unit_shape.setTexture(Resource_holder::get_resources().get_texture(texture_filename));
		this->x = x;
		this->y = y;
		can_jump = false;
		unit_shape.setPosition(sf::Vector2f(32.f * y, 32.f * x));
	}
	void allow_jump(bool allow)
	{
		can_jump = allow;
	}
	void update_position(float deltaTime, Unit* player);
	void update_position(float deltaTime) {};
	std::vector<std::unique_ptr<Bullet>>* get_bullets() { return nullptr; };
	void collision_up(Unit* object);
	void collision_down(Unit* object);
	void collision_left(Unit* object);
	void collision_right(Unit* object);
	void change_direction(direction dir);
};