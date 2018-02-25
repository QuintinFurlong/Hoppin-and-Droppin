#pragma once

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "EnemyGun.h"
#include "Colour.h"

class Enemies
{
public:
	static const int MAX_ENEMIES = 10;
	static const int MAX_MOVE = 1;
	
private:
	std::vector<sf::RectangleShape>  bodies;
	bool alive[MAX_ENEMIES];
	bool toTheRight[MAX_ENEMIES];
	sf::Vector2f velo[MAX_ENEMIES];
	EnemyGun gun[MAX_ENEMIES];
	int realMax = MAX_ENEMIES;
	sf::SoundBuffer gotShotBuffer;
	sf::Sound gotShot;

	void moveMent(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks);
	void deathAnimation(std::vector<sf::RectangleShape> t_blocks);

public:
	Enemies();
	void create(std::vector<EnemyData> t_enemyData);
	int update(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets, std::vector<sf::Vector2f> t_bulletVelo);
	void render(sf::RenderWindow & t_window);

	bool hit(sf::RectangleShape t_player);
	bool getAlive(int t_index);
	sf::RectangleShape getBody(int t_index);
	int getRealSize();
};