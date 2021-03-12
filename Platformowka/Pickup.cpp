#include <iostream>
#include "Pickup.h"

using namespace std;

bool Pickup::collision_check(Unit* object)
{
	if (this->pickup_sprite.getGlobalBounds().intersects(object->get_unit().getGlobalBounds()))
		return true;
	return false;
}
