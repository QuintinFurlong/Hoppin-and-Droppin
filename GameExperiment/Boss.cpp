#include "Boss.h"

void Boss::moveMent(std::vector<sf::RectangleShape> t_blocks)
{
	if (velo.x < -MAX_MOVE * 8 || velo.x > MAX_MOVE * 8)
	{
		velo.x *= .9;
	}
	else
	{
		velo.x *= 1.5;
	}
	velo.y += .98;

	for (int index = 0; index < t_blocks.size(); index++)
	{
		if (t_blocks.at(index).getFillColor() == FLOOR_COLOUR || t_blocks.at(index).getFillColor() == PLATFORM_COLOUR)
		{
			if (body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()) && velo.y >= 0 &&
				body.getPosition().y + body.getSize().y - t_blocks.at(index).getPosition().y < velo.y)
			{
				body.setPosition(sf::Vector2f(body.getPosition().x, t_blocks.at(index).getGlobalBounds().top - body.getSize().y));
				velo.y = 0;
			}
		}
	}

	body.move(velo);
	//wall collision
	for (int index = 0; index < t_blocks.size(); index++)
	{
		if (t_blocks.at(index).getFillColor() == WALL_COLOUR)
		{
			if (body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
			{
				if (body.getPosition().x > t_blocks.at(index).getPosition().x && body.getPosition().x < t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x)
				{
					body.setPosition(t_blocks.at(index).getPosition().x + t_blocks.at(index).getSize().x, body.getPosition().y);
					velo.x = MAX_MOVE;
				}
				else if (body.getPosition().x < t_blocks.at(index).getPosition().x && body.getPosition().x + body.getSize().x > t_blocks.at(index).getPosition().x)
				{
					body.setPosition(t_blocks.at(index).getPosition().x - body.getSize().x, body.getPosition().y);
					velo.x = -MAX_MOVE;
				}
			}
		}
	}
}

Boss::Boss()
{
	health = 1;
	velo.x = MAX_MOVE;
}

void Boss::create(BossData t_bossData)
{
	body.setPosition(t_bossData.m_position);
	body.setSize(t_bossData.m_size);
	if (t_bossData.m_type == "wall")
	{
		body.setFillColor(BOSS_WALL_COLOUR);
		health = WALL_HEALTH;
	}
	deadBody = body;
	deadBody.setSize(sf::Vector2f(body.getSize().x, 0));
	deadBody.setFillColor(sf::Color(body.getFillColor().r / 2, body.getFillColor().g / 2, body.getFillColor().b/2));
}

int Boss::update(std::vector<sf::RectangleShape> t_blocks, std::vector<sf::RectangleShape> t_bullets)
{
	int temp = -1;
	for (int bullies = 0; bullies < t_bullets.size(); bullies++)
	{
		if (body.getGlobalBounds().intersects(t_bullets.at(bullies).getGlobalBounds()))
		{
			health--;
			temp = bullies;
		}
	}

	moveMent(t_blocks);

	deadBody.setPosition(body.getPosition());
	if (health >= 0)
	{
		deadBody.setSize(sf::Vector2f(body.getSize().x, (body.getSize().y / WALL_HEALTH) * (WALL_HEALTH - health)));
	}
	return temp;
}

void Boss::render(sf::RenderWindow & t_window)
{
	t_window.draw(body);
	t_window.draw(deadBody);
}

sf::RectangleShape Boss::getBody()
{
	return body;
}
