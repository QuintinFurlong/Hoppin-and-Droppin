#include "Enemies.h"

Enemies::Enemies()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		bodies[i].setSize(sf::Vector2f(100, 150));
		bodies[i].setFillColor(sf::Color::Red);
		alive[i] = false;
		velo[i] = sf::Vector2f(0, 0);
		bodies[i].setOutlineThickness(1);
		bodies[i].setOutlineColor(sf::Color::White);
	}
}

void Enemies::create(std::vector<EnemyData> t_enemyData)
{
	for (int i = 0; i < t_enemyData.size(); i++)
	{
		bodies[i].setPosition(t_enemyData.at(i).m_position);
		alive[i] = true;
	}
}

void Enemies::update(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets)
{
	for (int enmies = 0; enmies < MAX_ENEMIES; enmies++)
	{
		for (int bullies = 0; bullies < t_bullets.size(); bullies++)
		{
			if (bodies[enmies].getGlobalBounds().intersects(t_bullets.at(bullies).getGlobalBounds()))
			{
				alive[enmies] = false;
				bodies[enmies].setOrigin(bodies[enmies].getGlobalBounds().width, bodies[enmies].getGlobalBounds().top);
				bodies[enmies].setRotation(90);
			}
		}
	}
	moveMent(t_player, t_blocks);
}

void Enemies::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		t_window.draw(bodies[i]);
	}
}

void Enemies::moveMent(sf::RectangleShape t_player, std::vector<sf::RectangleShape> t_blocks)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (alive[i])
		{
			velo[i].x *= .9;
			velo[i].y += .98;
			if (t_player.getPosition().x + t_player.getSize().x < bodies[i].getPosition().x)
			{
				velo[i].x -= MAX_MOVE;
			}
			else if (t_player.getPosition().x > bodies[i].getPosition().x + bodies[i].getSize().x)
			{
				velo[i].x += MAX_MOVE;
			}

			for (int index = 0; index < t_blocks.size(); index++)
			{
				if (t_blocks.at(index).getFillColor() == sf::Color::Green || t_player.getPosition().y < bodies[i].getPosition().y + bodies[i].getSize().y)
				{
					if (bodies[i].getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
					{
						if (velo[i].y >= 0)
						{
							if ((bodies[i].getPosition().y + bodies[i].getSize().y - t_blocks.at(index).getPosition().y < velo[i].y))
							{
								bodies[i].setPosition(sf::Vector2f(bodies[i].getPosition().x, t_blocks.at(index).getGlobalBounds().top - bodies[i].getSize().y));
								velo[i].y = 0;
							}
						}
					}
				}
			}

			if (t_player.getPosition().y + t_player.getSize().y < bodies[i].getPosition().y && velo[i].y == 0)//jump 
			{
				velo[i].y = -bodies[i].getSize().y / 6;
			}

			bodies[i].setPosition(bodies[i].getPosition() + velo[i]);
			for (int comp = 0; comp < MAX_ENEMIES; comp++)
			{
				if (alive[comp] && i != comp)
				{
					if (bodies[i].getGlobalBounds().intersects(bodies[comp].getGlobalBounds()))
					{
						if (bodies[i].getPosition().x > bodies[comp].getPosition().x)
						{
							bodies[i].setPosition(bodies[i].getPosition().x + MAX_MOVE, bodies[i].getPosition().y);
						}
						else if (bodies[i].getPosition().x < bodies[comp].getPosition().x)
						{
							bodies[i].setPosition(bodies[i].getPosition().x - MAX_MOVE, bodies[i].getPosition().y);
						}
					}
				}
			}
		}
	}
}