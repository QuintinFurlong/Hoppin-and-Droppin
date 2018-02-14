#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
	static const int CLIP_SIZE = 10;
	static const int FIRE_RATE = 30;

private:
	sf::RectangleShape m_body;
	std::vector<sf::RectangleShape> bullet;
	std::vector<sf::Vector2f> bulletVelo;
	int currentBullet;
	int timer;
	sf::Vector2f m_velo;

public:
	Player();
	void update();
	void render(sf::RenderWindow & t_window);
	sf::RectangleShape getBody();
	sf::Vector2f getVelo();
	void setPosition(sf::Vector2f t_vector);
	void stopFalling();
	std::vector<sf::RectangleShape> getBullets();
	std::vector<sf::Vector2f> getBulletVelo();
};