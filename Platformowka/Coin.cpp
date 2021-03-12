#include "Coin.h"

using namespace std;

const string Coin::texture_filename = "images/Coin.png";

void Coin::player_pickup(Player& player)
{
	player.add_points(value);
}
