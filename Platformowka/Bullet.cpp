#include "Bullet.h"

void Bullet::move_bullet(float deltaTime)
{
	velocity.x = 0.f;
	velocity.x += speed;

	bullet_sprite.move(velocity * deltaTime);
	distance_flown += abs(velocity.x * deltaTime); // zwiekszenie odleglosci, jaka pocisk przelecial
}
