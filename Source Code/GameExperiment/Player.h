#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Colour.h"

class Player
{
public:
	static const int CLIP_SIZE = 10;
	static const int FIRE_RATE = 30;
	static const int MAX_HEALTH = 10;

private:
	sf::RectangleShape m_body;
	std::vector<sf::RectangleShape> bullet;
	std::vector<sf::Vector2f> bulletVelo;
	int currentBullet;
	int timer;
	sf::Vector2f m_velo;
	int health;
	sf::RectangleShape m_healthBar;
	sf::SoundBuffer gunShotBuffer;
	sf::Sound gunShot;
	sf::SoundBuffer gotShotBuffer;
	sf::Sound gotShot;
	sf::SoundBuffer gotHitBuffer;
	sf::Sound gotHit;

public:
	Player();
	void update(std::vector<sf::RectangleShape> t_blocks);
	void render(sf::RenderWindow & t_window);
	sf::RectangleShape getBody();
	sf::Vector2f getVelo();
	void setPosition(sf::Vector2f t_vector);
	void stopFalling();
	std::vector<sf::RectangleShape> getBullets();
	std::vector<sf::Vector2f> getBulletVelo();
	void hit(int t_hit);
	void takeDamage(bool t_hit);
	int getHealth();
	void reset(sf::Vector2f t_pos);
	void bossTouch(sf::RectangleShape t_boss);
	void kill();
};