#include <iostream>
#include "Cowboy.h"
#include "Player.h"

using namespace std;

const string Cowboy::texture_filename = "images/Cowboy.png";

void Cowboy::update_position(float deltaTime, Unit* player)
{
	if ((player->get_position().x - this->get_position().x) > 0.f && dir != direction::right)  // obracanie sie w strone gracza
		this->change_direction_right();
	else if((player->get_position().x - this->get_position().x) <= 0.f && dir != direction::left)
		this->change_direction_left();
	if(fabs(player->get_position().x - this->get_position().x) < 250.f && fabs(player->get_position().y - this->get_position().y) < 5.f)
		shoot_bullet();  // jezeli gracz jest w odleglosci 250 pikseli w osi x, i mniej niz 5 pikseli w osi y, strzelaj
	velocity.x = 0.f;
	velocity.x += speed;
	if (velocity.y > 0.f)
		can_jump = false;
	velocity.y += 981.f * deltaTime;

	if (can_jump) 
		jump(this->jump_height);
	move_unit(velocity * deltaTime);
}

void Cowboy::collision_up(Unit* object)
{
	object->set_velocity_y(10.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_size().y + this->get_position().y + 0.1f));
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}

void Cowboy::collision_down(Unit* object)
{
	object->set_velocity_y(0.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_position().y - object->get_size().y - 0.1f));
	object->allow_jump(true);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}

void Cowboy::collision_left(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x + this->get_size().x + 0.1f, object->get_position().y));
	object->change_direction(direction::right);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}

void Cowboy::collision_right(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x - object->get_size().x - 0.1f, object->get_position().y));
	object->change_direction(direction::left);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}

void Cowboy::change_direction(direction dir)
{
	if (dir == direction::left)
		change_direction_left();
	else
		change_direction_right();
}

void Cowboy::shoot_bullet()
{
	if (shoot_timer.getElapsedTime().asSeconds() > 1.f) // minimum sekunda odstepu miedzy strzalami
	{
		enemy_bullets.push_back(make_unique<Bullet>()); // dodaj nowy pocisk 
		if (dir == direction::right)
			enemy_bullets.back()->set_position(sf::Vector2f(this->get_position().x + this->get_size().x, this->get_position().y)); 
		else  // ustawianie pozycji poczatek i kierunku w ktorym ma byc strzelane
		{
			enemy_bullets.back()->set_position(sf::Vector2f(this->get_position().x - 8.f, this->get_position().y));
			enemy_bullets.back()->change_direction();
		}
		shoot_timer.restart();
	}
}
