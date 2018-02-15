#include "Game.h"
#include <iostream>

Game::Game()
	: m_window{ sf::VideoMode{ 1400, 800 }, "Experiment" }
{
	if (!LevelLoader::load(1, currentLevel))
	{
		return;
	}

	m_enemies.create(currentLevel.m_enemies);

	for (WorldData const & obstacle : currentLevel.m_worldPieces)
	{
		sf::RectangleShape sprite;
		sprite.setPosition(obstacle.m_position);
		sprite.setSize(obstacle.m_size);
		if (obstacle.m_type == "floor")
			sprite.setFillColor(sf::Color::Green);
		else
			sprite.setFillColor(sf::Color(139, 69, 19));
		m_wallSprites.push_back(sprite);
	}
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
	}
}

void Game::update(sf::Time t_time)
{
	m_player.update();
	m_player.hit(m_enemies.update(m_player.getBody(), m_wallSprites, m_player.getBullets(), m_player.getBulletVelo()));
	m_player.takeDamage(m_enemies.hit(m_player.getBody()));

	m_view.setCenter(m_player.getBody().getPosition() +m_player.getBody().getSize() / 2.0f
		+ sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) * 3, sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) * 3));
	m_view.setSize(1400, 800);
	m_window.setView(m_view);

	for (int index = 0; index < m_wallSprites.size();index++)
	{
		//checks if platform and if left thumb stick is pointing down
		if (m_wallSprites.at(index).getFillColor() == sf::Color::Green || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < 50)
		{
			if (m_player.getBody().getGlobalBounds().intersects(m_wallSprites.at(index).getGlobalBounds())  //if player collids with objects and if player is going down
				)//ask me about it murt, i just cant in text
			{
				if (m_player.getVelo().y >= 0)
				{
					if ((m_player.getBody().getPosition().y + m_player.getBody().getSize().y - m_wallSprites.at(index).getPosition().y < m_player.getVelo().y))
					{
						m_player.setPosition(sf::Vector2f(m_player.getBody().getPosition().x, m_wallSprites.at(index).getGlobalBounds().top - m_player.getBody().getSize().y));
						m_player.stopFalling();
					}
					
				}
			}
		}
	}
}

void Game::render()
{
	m_window.clear();

	m_enemies.render(m_window);
	m_player.render(m_window);
	
	for (const auto &m_wallVector : m_wallSprites)
	{
		m_window.draw(m_wallVector);
	}

	m_window.display();
}
