#include "Game.h"
#include <iostream>

Game::Game()
	: m_window{ sf::VideoMode{ 1400, 800 }, "Experiment" }
{
	if (!LevelLoader::load(1, currentLevel))
	{
		return;
	}

	for (WorldData const & obstacle : currentLevel.m_worldPieces)
	{
		sf::RectangleShape sprite;
		sprite.setPosition(obstacle.m_position);
		sprite.setSize(obstacle.m_size);
		if (obstacle.m_type == "floor")
			sprite.setFillColor(sf::Color::Green);
		else
			sprite.setFillColor(sf::Color(200,200,200));
		m_wallSprites.push_back(sprite);
	}

	for(int index = 0;index < CLIP_SIZE;index++)
	{
		bullet[index].setSize(sf::Vector2f(20, 20));
		bullet[index].setFillColor(sf::Color::Red);
		bulletVelo[index] = sf::Vector2f(0, 0);
	}
	currentBullet = 0;

	m_player.setFillColor(sf::Color::Blue);
	m_player.setSize(sf::Vector2f(100,150));
	m_player.setPosition(700,400);

	m_view = m_window.getView();
	m_velo = 0;
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
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_keyHandler.updateKey(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			m_keyHandler.updateKey(event.key.code, false);
			break;
		default:
			break;
		}
	}
}

void Game::update(sf::Time)
{
	if (m_keyHandler.isPressed(sf::Keyboard::D))
	{
		m_player.move(5, 0);
		m_view.setCenter(m_window.getView().getCenter().x + 5, m_window.getView().getCenter().y);
	}
	if (m_keyHandler.isPressed(sf::Keyboard::A))
	{
		m_player.move(-5, 0);
		m_view.setCenter(m_window.getView().getCenter().x - 5, m_window.getView().getCenter().y);
	}
	if (m_keyHandler.isPressed(sf::Keyboard::W))
	{
		m_velo = -m_player.getSize().y / 6;
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Space))
	{
		sf::Vector2f temp = sf::Vector2f();
		//bulletVelo[currentBullet] = 
	}

	m_player.move(0,m_velo);
	m_velo += .98;

	m_view.setCenter(m_player.getPosition());
	m_window.setView(m_view);

	for (int index = 0; index < m_wallSprites.size();index++)
	{
		if (m_player.getGlobalBounds().intersects(m_wallSprites.at(index).getGlobalBounds()))
		{
			m_player.setPosition(m_player.getPosition().x, m_wallSprites.at(index).getGlobalBounds().top - m_player.getSize().y);
			m_velo = 0;
		}
	}

}

void Game::render()
{
	m_window.clear();

	m_window.draw(m_player);
	for (const auto &m_wallVector : m_wallSprites)
	{
		m_window.draw(m_wallVector);
	}
	
	m_window.display();
}
