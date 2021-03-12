#include "Upgrade.h"

using namespace std;

const string Upgrade::texture_filename = "images/Upgrade.png";

void Upgrade::player_pickup(Player& player)
{
	player.add_points(50);
	//jezeli gracz jest obrocony w prawo, speed jest dodatni, wiec dodaj predkosc
	if (player.get_speed() > 0.f)
		player.set_speed(player.get_speed() + speed_increase);
	else player.set_speed(player.get_speed() - speed_increase); // w przeciwnym wypadku, odejmij predkosc (predkosc jest ujemna)
	player.set_jump_height(player.get_jump_height() + jump_increase);
}