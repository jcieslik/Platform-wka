#include <SFML/Graphics.hpp>
#include <iostream>
#include "Unit.h"

using namespace std;

void Unit::change_direction_left()
{
	//obrot sprite'a w lewo
	unit_shape.setScale(sf::Vector2f(-1.f, 1.f));
	unit_shape.setOrigin(sf::Vector2f(size.x, 0.f));
	set_speed(-abs(speed));
	this->dir = direction::left;
}

void Unit::change_direction_right()
{
	//obrot sprite'a w prawo
	unit_shape.setScale(sf::Vector2f(1.f, 1.f));
	unit_shape.setOrigin(sf::Vector2f(0.f, 0.f));
	set_speed(abs(speed));
	this->dir = direction::right;
}

void Unit::jump(float jump_height)
{
	velocity.y = -sqrt(2.0f * 981.f * jump_height);
	allow_jump(false);
}

direction Unit::collision_check(Unit* object)
{
	float dx = (this->get_position().x + this->size.x / 2) - (object->get_position().x + object->get_size().x / 2); //odleglosc w osi x miedzy obiektami
	float dy = (this->get_position().y + this->size.y / 2) - (object->get_position().y + object->get_size().y / 2); //odleglosc w osi y miedzy obiektami
	float width = (this->size.x + object->get_size().x) / 2;  // suma polowek szerokosci obiektow (odleglosc od srodkow w osi x)
	float height = (this->size.y + object->get_size().y) / 2;  // suma polowek wysokosci obiekow (odleglosc od srodkow w osi y)
	float crossWidth = width * dy;      // szerokosc przeciecia miedzy obiektami
	float crossHeight = height * dx;    // wysokosc przeciecia miedzy obiektami
	direction collision = direction::no_direction;
	if (abs(dx) <= width && abs(dy) <= height)   // test, czy obiekty sie przecinaja
	{ 
		if (crossWidth > crossHeight) // jezeli szerokosc przeciecia jest wieksza od wysokosci (wysokosc ujemna), to kolizja zaszla z dolu lub z lewej
			collision = (crossWidth > (-crossHeight)) ? direction::down : direction::left;
		else  // w przeciwnym razie, z prawej lub z gory
			collision = (crossWidth > (-crossHeight)) ? direction::right : direction::up;
	}
	return collision;
}
