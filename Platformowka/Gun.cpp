#include <random>
#include "Gun.h"

using namespace std;

const string Gun::texture_filename = "images/Gun.png";

void Gun::player_pickup(Player& player)
{
	player.add_points(50);
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(1, 7);
	int bullets = distribution(generator);
	player.add_bullets(bullets); // dodaje losowa ilosc pociskow, od 1 do 7
}
