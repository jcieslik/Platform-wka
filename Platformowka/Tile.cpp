#include "Tile.h"

direction Tile::collision_check(Unit* object)
{
	float dx = (this->get_position().x + this->size.x / 2) - (object->get_position().x + object->get_size().x / 2); //odleglosc w osi x miedzy obiektami
	float dy = (this->get_position().y + this->size.y / 2) - (object->get_position().y + object->get_size().y / 2); //odleglosc w osi y miedzy obiektami
	float width = (this->size.x + object->get_size().x) / 2;   // suma polowek szerokosci obiektow (odleglosc od srodkow w osi x)
	float height = (this->size.y + object->get_size().y) / 2;  // suma polowek wysokosci obiekow (odleglosc od srodkow w osi y)
	float crossWidth = width * dy;                           // szerokosc przeciecia miedzy obiektami
	float crossHeight = height * dx;                         // wysokosc przeciecia miedzy obiektami
	direction collision = direction::no_direction;
	if (abs(dx) <= width && abs(dy) <= height)      // test, czy obiekty sie przecinaja
	{
		if (crossWidth > crossHeight) // jezeli szerokosc przeciecia jest wieksza od wysokosci (wysokosc ujemna), to kolizja zaszla z dolu lub z lewej
			collision = (crossWidth > (-crossHeight)) ? direction::down : direction::left;
		else   // w przeciwnym razie, z prawej lub z gory
			collision = (crossWidth > (-crossHeight)) ? direction::right : direction::up;
	}
	return collision;  
}