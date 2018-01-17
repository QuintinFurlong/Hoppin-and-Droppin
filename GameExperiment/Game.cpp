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
	timer = 0;
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
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 30)
	{
		m_player.move(10, 0);
	}
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -30)
	{
		m_player.move(-10, 0);
	}
	if (sf::Joystick::isButtonPressed(0, 0))
	{
		m_velo = -m_player.getSize().y / 6;
	}
	if (timer == 0)
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) < -1)
		{
			sf::Vector2f temp = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R));
			temp = sf::Vector2f(temp.x / sqrt(temp.x * temp.x + temp.y * temp.y), temp.y / sqrt(temp.x * temp.x + temp.y * temp.y));
			bulletVelo[currentBullet] = temp * 20.0f;
			bullet[currentBullet].setPosition(m_player.getPosition() + m_player.getSize()/2.0f);
			currentBullet++;
			bulletVelo[currentBullet] = sf::Vector2f(0,0);
			timer = FIRE_RATE;
			std::cout << temp.x << "---" << temp.y << std::endl;
		}
	}
	else
	{
		timer--;
	}
	if (currentBullet > CLIP_SIZE)
	{
		currentBullet = 0;
	}

	m_player.move(0,m_velo);
	m_velo += .98;
	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo[index] != sf::Vector2f(0,0))
		{
			bullet[index].setPosition(bullet[index].getPosition() + bulletVelo[index]);
		}
	}

	m_view.setCenter(m_player.getPosition());
	m_window.setView(m_view);

	for (int index = 0; index < m_wallSprites.size();index++)
	{
		//checks if platform and if left thumb stick is pointing down
		if (m_wallSprites.at(index).getFillColor() != sf::Color(200, 200, 200) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < 50)
		{
			if (m_player.getGlobalBounds().intersects(m_wallSprites.at(index).getGlobalBounds()) && m_velo >= 0 //if player collids with objects and if player is going down
				&& (m_player.getPosition().y + m_player.getSize().y - m_wallSprites.at(index).getPosition().y < m_velo))//ask me about it murt, i just cant in text
			{
				m_player.setPosition(m_player.getPosition().x, m_wallSprites.at(index).getGlobalBounds().top - m_player.getSize().y);
				m_velo = 0;
			}
		}
	}

}

void Game::render()
{
	m_window.clear();

	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo[index] != sf::Vector2f(0, 0))
		{
			m_window.draw(bullet[index]);
		}
	}

	m_window.draw(m_player);
	for (const auto &m_wallVector : m_wallSprites)
	{
		m_window.draw(m_wallVector);
	}
	
	m_window.display();
}
