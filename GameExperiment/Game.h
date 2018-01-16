#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "KeyHandler.h"

class Game
{
public:
	Game();
	void run();
private:
	void             processEvents();
	void             update(sf::Time);
	void             render();

	sf::RenderWindow m_window;
	sf::RectangleShape m_player;
	static const int CLIP_SIZE = 10;
	sf::RectangleShape bullet[CLIP_SIZE];
	sf::Vector2f bulletVelo[CLIP_SIZE];
	int currentBullet;
	float m_velo;
	sf::View m_view;

	LevelData currentLevel;
	std::vector<sf::RectangleShape> m_wallSprites;
	std::vector<sf::RectangleShape *> m_wallVector;
	KeyHandler m_keyHandler;
};

#endif // !GAME

