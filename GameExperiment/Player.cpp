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
	m_body.setPosition(300, 100);

	m_healthBar.setFillColor(sf::Color::Magenta);
	m_healthBar.setSize(sf::Vector2f(100, 0));
	m_healthBar.setPosition(300, 100);

	m_velo = sf::Vector2f(0, 0);
	timer = 0;
	health = MAX_HEALTH;
}

void Player::update(std::vector<sf::RectangleShape> t_blocks)
{
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20)
	{
		m_velo.x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100;
	}
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20)
	{
		m_velo.x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 100;
	}
	for (int index = 0; index < t_blocks.size(); index++)
	{
		//checks if platform and if left thumb stick is pointing down
		if (t_blocks.at(index).getFillColor() == FLOOR_COLOUR || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < 50)
		{
			if (m_body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds())  //if player collids with objects and if player is going down
				)//ask me about it murt, i just cant in text
			{
				if (m_velo.y >= 0)
				{
					if ((m_body.getPosition().y + m_body.getSize().y - t_blocks.at(index).getPosition().y < m_velo.y))
					{
						m_body.setPosition(sf::Vector2f(m_body.getPosition().x, t_blocks.at(index).getGlobalBounds().top - m_body.getSize().y));
						stopFalling();
					}

				}
			}
		}
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
	
	for (int index = 0; index < t_blocks.size(); index++)
	{
		if (t_blocks.at(index).getFillColor() == WALL_COLOUR)//wall colision
		{
			if (m_body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
			{
				if(m_body.getPosition().x > t_blocks.at(index).getPosition().x)// && m_body.getPosition().x < t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x)
				{
					m_body.setPosition(t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x, m_body.getPosition().y);
				}
				else
				{
					m_body.setPosition(t_blocks.at(index).getPosition().x - m_body.getSize().x, m_body.getPosition().y);
				}
			}
		}
		else if (t_blocks.at(index).getFillColor() == ROOF_COLOUR)//roof colision
		{
			if (m_body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
			{
				m_body.setPosition(m_body.getPosition().x, t_blocks.at(index).getPosition().y + t_blocks.at(index).getSize().y);
				m_velo.y = 0.1;//zero allow jumping when touching roof
			}
		}
	}

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
	m_healthBar.setPosition(m_body.getPosition());
	if (health >= 0)
	{
		m_healthBar.setSize(sf::Vector2f(m_body.getSize().x, (m_body.getSize().y / MAX_HEALTH) * (MAX_HEALTH - health)));
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
	t_window.draw(m_healthBar);
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

void Player::hit(int t_hit)
{
	if (t_hit != -1)
	{
		bullet.at(t_hit).setPosition(-1000, -1000);
		bulletVelo.at(t_hit) = sf::Vector2f(0, 0);
	}
}

void Player::takeDamage(bool t_hit)
{
	if (t_hit)
	{
		health--;
	}
}

int Player::getHealth()
{
	return health;
}

void Player::reset(sf::Vector2f t_pos)
{
	m_body.setPosition(t_pos);
	m_velo = sf::Vector2f(0, 0);
	timer = 0;
	health = MAX_HEALTH;
}

void Player::bossTouch(sf::RectangleShape t_boss)
{
	if(m_body.getGlobalBounds().intersects(t_boss.getGlobalBounds()))
	{
		health--;
		if(m_body.getPosition().x + m_body.getSize().x/2 > t_boss.getPosition().x + t_boss.getSize().x / 2)
		{
			m_velo.x = 30;
		}
		else
		{
			m_velo.x = -30;
		}
		if (m_body.getPosition().y + m_body.getSize().y / 2 < t_boss.getPosition().y + t_boss.getSize().y / 2)
		{
			m_velo.y = -30;
		}
	}
}
