#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LevelLoader.h"
#include "Colour.h"

class Boss
{
public:
	static const int MAX_MOVE = 1;
	static const int WALL_HEALTH = 20;
	static const int SPIDER_HEALTH = 3;
	static const int BIRD_HEALTH = 5;
	static const int MAX_ROTATION = 45;
	static const int KANGAROO_HEALTH = 4;
	static const int MUTANT_HEALTH = 10;
	static const int TELEPORT_COOLDOWN = 200;

private:
	sf::RectangleShape  body;
	sf::RectangleShape deadBody;
	bool alive;
	sf::Vector2f velo;
	int health;
	sf::RectangleShape wing;
	sf::RectangleShape deadWing;
	bool rotateUp;
	int timer;
	bool midTeleport;
	sf::SoundBuffer gotShotBuffer;
	sf::Sound gotShot;

	void moveMent(std::vector<sf::RectangleShape> t_blocks);

public:
	Boss();
	void create(BossData t_bossData);
	int update(std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets);
	void render(sf::RenderWindow & t_window);

	int getHealth();
	sf::RectangleShape getBody();
	void flying(std::vector<sf::RectangleShape> t_blocks);
	void teleporting(std::vector<sf::RectangleShape> t_blocks);
};