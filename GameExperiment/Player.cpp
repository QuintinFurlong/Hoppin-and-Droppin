#include "Player.h"

Player::Player()
{
	for (int index = 0; index < CLIP_SIZE; index++)
	{
		sf::RectangleShape tempBullet;
		tempBullet.setSize(sf::Vector2f(20, 20));
		tempBullet.setFillColor(sf::Color(192, 192, 192));
		sf::Vector2f tempVelo;
		tempVelo = sf::Vector2f(0, 0);
		bullet.push_back(tempBullet);
		bulletVelo.push_back(tempVelo);
	}
	currentBullet = 0;

	m_body.setFillColor(sf::Color::Blue);
	m_body.setSize(sf::Vector2f(100, 150));
	m_body.setPosition(700, 400);

	m_velo = sf::Vector2f(0, 0);
	timer = 0;
}

void Player::update()
{
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20)
	{
		m_velo.x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100;
	}
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20)
	{
		m_velo.x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100;
	}
	if (sf::Joystick::isButtonPressed(0, 0) && m_velo.y == 0)//jump (A)
	{
		m_velo.y = -m_body.getSize().y / 6;
	}
	if (timer == 0)
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) < -1)//right trigger shoot
		{
			sf::Vector2f temp = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R));
			temp = sf::Vector2f(temp.x / sqrt(temp.x * temp.x + temp.y * temp.y), temp.y / sqrt(temp.x * temp.x + temp.y * temp.y));
			bulletVelo[currentBullet] = temp * 20.0f;
			m_velo -= bulletVelo[currentBullet];
			bullet.at(currentBullet).setPosition(m_body.getPosition() + m_body.getSize() / 2.0f);
			currentBullet++;
			if (currentBullet >= CLIP_SIZE)
			{
				currentBullet = 0;
			}
			bulletVelo[currentBullet] = sf::Vector2f(0, 0);
			timer = FIRE_RATE;
		}
	}
	else
	{
		timer--;
	}

	m_body.move(m_velo);
	m_velo.x *= .9;
	m_velo.y += .98;
	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo[index] != sf::Vector2f(0, 0))
		{
			bullet.at(index).setPosition(bullet.at(index).getPosition() + bulletVelo[index]);
		}
		if(sqrt(pow(m_body.getPosition().x - bullet[index].getPosition().x, 2) + pow(m_body.getPosition().y - bullet[index].getPosition().y, 2)) > 1100)
		{
			bullet.at(index).setPosition(-1000, -1000);
			bulletVelo[index] = sf::Vector2f(0, 0);
		}
	}
}

void Player::render(sf::RenderWindow & t_window)
{

	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo[index] != sf::Vector2f(0, 0))
		{
			t_window.draw(bullet.at(index));
		}
	}

	t_window.draw(m_body);
}

sf::RectangleShape Player::getBody()
{
	return m_body;
}

sf::Vector2f Player::getVelo()
{
	return m_velo;
}

void Player::setPosition(sf::Vector2f t_vector)
{
	m_body.setPosition(t_vector);
}

void Player::stopFalling()
{
	m_velo.y = 0;
}

std::vector<sf::RectangleShape> Player::getBullets()
{
	return bullet;
}

std::vector<sf::Vector2f> Player::getBulletVelo()
{
	return bulletVelo;
}

void Player::hit(bool t_hit)
{
	if (t_hit)
	{
		if (currentBullet != 0)
		{
			bullet.at(currentBullet - 1).setPosition(-1000, -1000);
			bulletVelo.at(currentBullet - 1) = sf::Vector2f(0, 0);
		}
		else
		{
			bullet.at(currentBullet - 1+ CLIP_SIZE).setPosition(-1000, -1000);
			bulletVelo.at(currentBullet - 1+ CLIP_SIZE) = sf::Vector2f(0, 0);
		}
	}
}

void Player::takeDamage(bool t_hit)
{
	if (t_hit)
	{
		m_body.setFillColor(sf::Color(m_body.getFillColor().r, m_body.getFillColor().g, m_body.getFillColor().b, m_body.getFillColor().a - 5));
	}
}
