#include "Game.h"
#include <iostream>

Game::Game()
	: m_window{ sf::VideoMode{ 1400, 800 }, "Experiment" }
{
	srand(time(NULL));
	levelNumber = 5;
	if (!LevelLoader::load(levelNumber, currentLevel))
	{
		return;
	}
	m_player.setPosition(currentLevel.m_player.m_position);
	m_player.kill();
	m_enemies.create(currentLevel.m_enemies);
	m_boss.create(currentLevel.m_boss);

	for (WorldData const & obstacle : currentLevel.m_worldPieces)
	{
		sf::RectangleShape sprite;
		sprite.setPosition(obstacle.m_position);
		sprite.setSize(obstacle.m_size);
		if (obstacle.m_type == "floor")
			sprite.setFillColor(FLOOR_COLOUR);
		else if (obstacle.m_type == "wall") 
			sprite.setFillColor(WALL_COLOUR);
		else if(obstacle.m_type == "roof")
			sprite.setFillColor(ROOF_COLOUR);
		else
			sprite.setFillColor(PLATFORM_COLOUR);
		m_wallSprites.push_back(sprite);
	}
	m_offsetView = sf::Vector2f(0, 0);
	if (!musicBuffer.loadFromFile("ASSETS/SOUNDS/music.wav"))
	{
		//err
	}
	music.setBuffer(musicBuffer);
	music.setLoop(true);
	music.setVolume(10);
	music.play();
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	while (m_window.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		if (sf::Joystick::isButtonPressed(0, 6) && m_gamestate == GameState::GamePlay)
		{
			m_gamestate = GameState::MainMenu;
		}
	}
}

void Game::update(sf::Time t_time)
{
	if (m_gamestate == GameState::MainMenu)
	{
		m_mainMenu.update(t_time, m_gamestate);
	}
	else if (m_gamestate == GameState::Levels)
	{
		m_level.update(t_time, m_gamestate, levelNumber);
		if (m_gamestate == GameState::GamePlay) 
		{
			currentLevel.m_enemies.clear();
			currentLevel.m_worldPieces.clear();
			if (!LevelLoader::load(levelNumber, currentLevel))
			{
				return;
			}
			m_player.reset(currentLevel.m_player.m_position);

			m_enemies.create(currentLevel.m_enemies);
			m_boss.create(currentLevel.m_boss);
			m_wallSprites.clear();
			for (WorldData const & obstacle : currentLevel.m_worldPieces)
			{
				sf::RectangleShape sprite;
				sprite.setPosition(obstacle.m_position);
				sprite.setSize(obstacle.m_size);
				if (obstacle.m_type == "floor")
					sprite.setFillColor(FLOOR_COLOUR);
				else if (obstacle.m_type == "wall")
					sprite.setFillColor(WALL_COLOUR);
				else if (obstacle.m_type == "roof")
					sprite.setFillColor(ROOF_COLOUR);
				else
					sprite.setFillColor(PLATFORM_COLOUR);
				m_wallSprites.push_back(sprite);
			}
		}
	}
	else if (m_gamestate == GameState::GamePlay)//if game mode do all game stuff
	{
		m_player.update(m_wallSprites);
		m_player.hit(m_enemies.update(m_player.getBody(), m_wallSprites, m_player.getBullets(), m_player.getBulletVelo()));
		m_player.hit(m_boss.update(m_wallSprites, m_player.getBullets()));
		m_player.takeDamage(m_enemies.hit(m_player.getBody()));
		m_player.bossTouch(m_boss.getBody());

		sf::Vector2f temp(m_player.getBody().getPosition() + m_player.getBody().getSize() / 2.0f
			+ sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) * 3, sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) * 3));

		if (m_offsetView != sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) * 3, sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) * 3))
		{
			m_offsetView += sf::Vector2f((temp.x - m_view.getCenter().x) / CAM_SPEED, (temp.y - m_view.getCenter().y) / CAM_SPEED);
		}
		m_view.setCenter(m_player.getBody().getPosition() + m_player.getBody().getSize() / 2.0f + m_offsetView);
		m_view.setSize(1400, 800);
		m_window.setView(m_view);

		//for jumping on bodies

		for (int index = 0; index < m_enemies.getRealSize(); index++)
		{
			//checks if enemy is dead and stick not pointing down
			if (!m_enemies.getAlive(index) && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < 50)
			{
				if (m_player.getBody().getGlobalBounds().intersects(m_enemies.getBody(index).getGlobalBounds()))  //if player collids with enemy
				{
					if (m_player.getVelo().y >= 0)//player moving down
					{
						if (m_enemies.getBody(index).getRotation() == 90)
						{
							if (m_player.getBody().getPosition().y + m_player.getBody().getSize().y - m_enemies.getBody(index).getPosition().y < m_player.getVelo().y)
							{
								m_player.setPosition(sf::Vector2f(m_player.getBody().getPosition().x,
									m_enemies.getBody(index).getGlobalBounds().top - m_player.getBody().getSize().y));
								m_player.stopFalling();
							}
						}
						else if (m_enemies.getBody(index).getRotation() == 270)
						{
							if (m_player.getBody().getPosition().y + m_player.getBody().getSize().y -
								m_enemies.getBody(index).getPosition().y + m_enemies.getBody(index).getSize().x < m_player.getVelo().y)
							{
								m_player.setPosition(sf::Vector2f(m_player.getBody().getPosition().x,
									m_enemies.getBody(index).getGlobalBounds().top - m_player.getBody().getSize().y));
								m_player.stopFalling();
							}
						}
					}

				}
			}
		}
		//player is dead
		if (m_player.getHealth() == 0 || m_boss.getHealth() == 0)
		{
			if (m_boss.getHealth() == 0 && levelNumber < 5)
			{
				levelNumber++;
			}
			currentLevel.m_enemies.clear();
			currentLevel.m_worldPieces.clear();
			if (!LevelLoader::load(levelNumber, currentLevel))
			{
				return;
			}
			m_player.reset(currentLevel.m_player.m_position);

			m_enemies.create(currentLevel.m_enemies);
			m_boss.create(currentLevel.m_boss);
			m_wallSprites.clear();
			for (WorldData const & obstacle : currentLevel.m_worldPieces)
			{
				sf::RectangleShape sprite;
				sprite.setPosition(obstacle.m_position);
				sprite.setSize(obstacle.m_size);
				if (obstacle.m_type == "floor")
					sprite.setFillColor(FLOOR_COLOUR);
				else if (obstacle.m_type == "wall")
					sprite.setFillColor(WALL_COLOUR);
				else if (obstacle.m_type == "roof")
					sprite.setFillColor(ROOF_COLOUR);
				else
					sprite.setFillColor(PLATFORM_COLOUR);
				m_wallSprites.push_back(sprite);
			}
		}
	}
}


void Game::render()
{
	m_window.clear(SKY_COLOUR);
	if (m_gamestate == GameState::MainMenu)//if main menu mode
	{
		m_view.setCenter(700,400);
		m_view.setSize(1400, 800);
		m_window.setView(m_view);
		m_mainMenu.render(m_window);//draw main menu
	}
	else if (m_gamestate == GameState::Levels)
	{
		m_view.setCenter(700, 400);
		m_view.setSize(1400, 800);
		m_window.setView(m_view);
		m_level.render(m_window);
	}
	else if (m_gamestate == GameState::GamePlay)//if gameplay mode draw game
	{
		m_enemies.render(m_window);
		m_boss.render(m_window);
		m_player.render(m_window);

		for (const auto &m_wallVector : m_wallSprites)
		{
			m_window.draw(m_wallVector);
		}
	}
	m_window.display();
}
