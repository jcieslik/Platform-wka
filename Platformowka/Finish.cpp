#include <SFML/Graphics.hpp>
#include <iostream>
#include "Finish.h"
#include "Rat.h"
#include "Player.h"

using namespace std;

const string Finish::texture_filename = "images/Finish.png";

void Finish::collision_up(Unit* object)
{
	object->set_velocity_y(10.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_size().y + this->get_position().y));
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->set_completed(true);
}
void Finish::collision_down(Unit* object)
{
	object->set_velocity_y(0.f);
	object->set_position(sf::Vector2f(object->get_position().x, this->get_position().y - object->get_size().y));
	object->allow_jump(true);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->set_completed(true);
}
void Finish::collision_left(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x + this->get_size().x + 0.1f, object->get_position().y));
	object->change_direction(direction::right);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->set_completed(true);
}
void Finish::collision_right(Unit* object)
{
	object->set_velocity_x(0.f);
	object->set_position(sf::Vector2f(this->get_position().x - object->get_size().x - 0.1f, object->get_position().y));
	object->change_direction(direction::left);
	if (dynamic_cast<Player*>(object))
		dynamic_cast<Player*>(object)->set_completed(true);
}