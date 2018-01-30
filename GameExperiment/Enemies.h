#pragma once

#include <SFML/Graphics.hpp>

class Enemies
{
public:
	static const int MAX_ENEMIES = 10;
	static const int MAX_MOVE = 5;

private:
	sf::RectangleShape bodies[MAX_ENEMIES];
	bool alive[MAX_ENEMIES];
	sf::Vector2f velo[MAX_ENEMIES];

public:
	Enemies();
	void update(sf::RectangleShape t_player);
	void render(sf::RenderWindow & t_window);

};