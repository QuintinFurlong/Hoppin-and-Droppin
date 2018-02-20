#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "Player.h"
#include "Enemies.h"
#include "Colour.h"

class Game
{
public:
	Game();
	void run();
private:
	void             processEvents();
	void             update(sf::Time t_time);
	void             render();

	sf::RenderWindow m_window;
	Player m_player;
	sf::View m_view;
	sf::Vector2f m_offsetView;
	static const int CAM_SPEED = 8;
	Enemies m_enemies;

	LevelData currentLevel;
	std::vector<sf::RectangleShape> m_wallSprites;
	std::vector<sf::RectangleShape *> m_wallVector;
};

#endif // !GAME

