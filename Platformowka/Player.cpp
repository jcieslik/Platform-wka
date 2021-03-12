#include <iostream>
#include "Player.h"
#include "Level.h"
#include "Unit.h"

using namespace std;

void Player::update_position(float deltaTime)
{
	velocity.x = 0.f;

	if (spawn_timer.getElapsedTime().asSeconds() > 1.f) // przez sekunde po pojawieniu sie nie mozna sie poruszac
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if(dir != direction::right)
				change_direction_right();
			velocity.x += speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (dir != direction::left)
				change_direction_left();
			velocity.x += speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && can_jump == true)
			jump(jump_height);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && bullets > 0)
			shoot_bullet();
	}
	if(velocity.y > 0.f)
		can_jump = false;
	velocity.y += 981.f * deltaTime;

	move_unit(velocity * deltaTime);
}

void Player::shoot_bullet()
{
	if (shoot_timer.getElapsedTime().asSeconds() > 1.5f)  // 1,5 sekundy odstepu miedzy strzalami
	{
		player_bullets.push_back(make_unique<Bullet>());
		if (dir == direction::right)
			player_bullets.back()->set_position(sf::Vector2f(this->get_position().x + this->get_size().x, this->get_position().y + this->get_size().y / 2));
		else  // sprawdzanie kierunku i pozycji poczatkowej pocisku
		{
			player_bullets.back()->set_position(sf::Vector2f(this->get_position().x - 8.f, this->get_position().y + this->get_size().y / 2));
			player_bullets.back()->change_direction();
		}
		shoot_timer.restart();
		bullets--;
	}
}

void Player::damage()
{
	if (spawn_timer.getElapsedTime().asSeconds() > 1.5f) // 1,5 sekundy czasu niesmiertelnosci
	{
		lives--;
		this->set_position(get_spawn_position());
		reset_spawn_timer();
	}
}


