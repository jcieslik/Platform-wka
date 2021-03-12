#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "enums.h"
#include "Bullet.h"
#include "Resource_holder.h"

class Unit : public sf::Drawable
{
protected:
	sf::Sprite unit_shape;
	sf::Vector2f size;
	sf::Vector2f velocity;
	direction dir;
	float speed;    // dodatni, jezeli idzie w prawo, w lewo ujemny
	float jump_height;
	bool can_jump;
	int points_worth;
	int x;
	int y;
public:
	Unit()
	{
		points_worth = 0;
		can_jump = false;
		jump_height = 0.f;
		dir = direction::right;
		speed = 0;
		x = 0;
		y = 0;
	}
	void draw(sf::RenderTarget& unit, sf::RenderStates state) const
	{
		unit.draw(unit_shape);
	}
	void move_unit(sf::Vector2f velocity)
	{
		unit_shape.move(velocity);
	}
	float get_speed()
	{
		return speed;
	}
	sf::Sprite get_unit()
	{
		return unit_shape;
	}
	sf::Vector2f get_position()
	{
		return unit_shape.getPosition();
	}
	sf::Vector2f get_velocity()
	{
		return velocity;
	}
	sf::Vector2f get_size()
	{
		return size;
	}
	void set_speed(float speed)
	{
		this->speed = speed;
	}
	void set_position(sf::Vector2f new_position)
	{
		unit_shape.setPosition(new_position);
	}
	void set_velocity_x(float vel)
	{
		velocity.x = vel;
	}
	void set_velocity_y(float vel)
	{
		velocity.y = vel;
	}
	direction get_direction()
	{
		return dir;
	}
	float get_jump_height()
	{
		return jump_height;
	}
	bool check_can_jump()
	{
		return can_jump;
	}
	int get_points_worth()
	{
		return points_worth;
	}
	void jump(float jump_height);
	direction collision_check(Unit* object);
	virtual void collision_up(Unit* object) = 0;
	virtual void collision_down(Unit* object) = 0;
	virtual void collision_left(Unit* object) = 0;
	virtual void collision_right(Unit* object) = 0;
	virtual void update_position(float deltaTime, Unit* player) = 0;
	virtual void update_position(float deltaTime) = 0;
	virtual void allow_jump(bool allow) = 0;
	virtual void change_direction(direction dir) = 0;
	virtual std::vector<std::unique_ptr<Bullet>>* get_bullets() = 0; 
	void change_direction_left();
	void change_direction_right();
};