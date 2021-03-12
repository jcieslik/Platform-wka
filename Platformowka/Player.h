#pragma once
#include <vector>
#include "Unit.h"

class Player : public Unit
{
private:
	sf::Vector2f spawn_position;
	sf::Time cooldown;
	sf::Clock spawn_timer;
	sf::Clock shoot_timer;
	std::vector<std::unique_ptr<Bullet>> player_bullets;
	int points;
	int lives;
	int bullets;
	bool can_jump;
	bool completed_level;
public:
	Player()
	{
		jump_height = 100.f;
		bullets = 0;
		points = 0;
		lives = 3;
		cooldown = sf::seconds(1.f);
		can_jump = true;
		completed_level = false;
		speed = 180.f; 
		size = sf::Vector2f(16.f, 20.f);
		unit_shape.setTexture(Resource_holder::get_resources().get_texture("images/Character.png"));
		unit_shape.setPosition(sf::Vector2f(32.f * y, 32.f * x));
	}
	bool get_completed()
	{
		return completed_level;
	}
	sf::Vector2f get_spawn_position()
	{
		return spawn_position;
	}
	void allow_jump(bool allow)
	{
		can_jump = allow;
	}
	void set_completed(bool completed)
	{
		completed_level = completed;
	}
	void set_spawn_position(sf::Vector2f spawn)
	{
		spawn_position = spawn;
	}
	void reset_spawn_timer()
	{
		spawn_timer.restart();
	}
	int get_lives()
	{
		return lives;
	}
	int& get_points()
	{
		return points;
	}
	void add_points(int add)
	{
		points += add;
	}
	void change_direction(direction dir)
	{
		set_velocity_x(0.f);
	};
	std::vector<std::unique_ptr<Bullet>>* get_bullets()
	{
		return &player_bullets;
	}
	void add_bullets(int bullets)
	{
		this->bullets += bullets;
	}
	void set_jump_height(float height)
	{
		jump_height = height;
	}
	int get_bullets_amount()
	{
		return bullets;
	}
	void update_position(float deltaTime);
	void update_position(float deltaTime, Unit* player) {};
	void shoot_bullet();
	void damage();
	void collision_up(Unit* object) {};
	void collision_down(Unit* object) {};
	void collision_left(Unit* object) {};
	void collision_right(Unit* object) {};
};