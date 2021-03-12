#include <iostream>
#include "Rat.h"
#include "Player.h"

using namespace std;

const string Rat::texture_filename = "images/Rat.png";

void Rat::update_position(float deltaTime, Unit* player)
{
	velocity.x = 0.f;
	velocity.x += speed;
	velocity.y += 981.f * deltaTime;

	move_unit(velocity * deltaTime);
}

void Rat::collision_up(Unit* object)
{
	object->set_velocity_y(10.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_size().y + this->get_position().y + 0.1f));
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}
void Rat::collision_down(Unit* object)
{
	object->set_velocity_y(0.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_position().y - object->get_size().y - 0.1f));
	object->allow_jump(true);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}
void Rat::collision_left(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x + this->get_size().x, object->get_position().y));
	object->change_direction(direction::right);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}
void Rat::collision_right(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x - object->get_size().x, object->get_position().y));
	object->change_direction(direction::left);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->damage();
}

void Rat::change_direction(direction dir)
{
	if (dir == direction::left)
		change_direction_left();
	else
		change_direction_right();
}