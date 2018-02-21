#pragma once

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "Colour.h"

class Boss
{
public:
	static const int MAX_MOVE = 1;
	static const int WALL_HEALTH = 20;

private:
	sf::RectangleShape  body;
	sf::RectangleShape deadBody;
	bool alive;
	sf::Vector2f velo;
	int health;

	void moveMent(std::vector<sf::RectangleShape> t_blocks);
	void deathAnimation(std::vector<sf::RectangleShape> t_blocks);

public:
	Boss();
	void create(BossData t_bossData);
	int update(std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets);
	void render(sf::RenderWindow & t_window);

	int hit(sf::RectangleShape t_player);
	int getHealth();
	sf::RectangleShape getBody();
};