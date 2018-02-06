#pragma once

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"

class Enemies
{
public:
	static const int MAX_ENEMIES = 10;
	static const int MAX_MOVE = 1;

private:
	sf::RectangleShape bodies[MAX_ENEMIES];
	bool alive[MAX_ENEMIES];
	sf::Vector2f velo[MAX_ENEMIES];

	void moveMent(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks);

public:
	Enemies();
	void create(std::vector<EnemyData> t_enemyData);
	void update(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets);
	void render(sf::RenderWindow & t_window);

};