#pragma once
#include <SFML/Graphics.hpp>
#include "Colour.h"

class EnemyGun
{
public:
	static const int CLIP_SIZE = 10;
	static const int FIRE_RATE = 60;

	EnemyGun();

	sf::Vector2f update(sf::RectangleShape t_playerPos, sf::RectangleShape t_enemy, bool t_enemyAlive);
	bool hit(sf::RectangleShape t_player);
	void render(sf::RenderWindow & t_window);
private:
	std::vector<sf::RectangleShape> bullet;
	std::vector<sf::Vector2f> bulletVelo;
	int currentBullet;
	int timer;
};
