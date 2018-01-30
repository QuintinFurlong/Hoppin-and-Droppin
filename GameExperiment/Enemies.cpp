#include "Enemies.h"

Enemies::Enemies()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		bodies[i].setSize(sf::Vector2f(100, 150));
		bodies[i].setFillColor(sf::Color::Red);
		alive[i] = false;
		velo[i] = sf::Vector2f(0, 0);
	}
}

void Enemies::update(sf::Vector2f t_playerPos)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		bodies[i].setPosition(bodies[i].getPosition() + velo[i]);
	}
}

void Enemies::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		t_window.draw(bodies[i]);
	}
}
