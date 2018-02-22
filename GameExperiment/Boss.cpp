#include "Boss.h"

void Boss::moveMent(std::vector<sf::RectangleShape> t_blocks)
{
	body.setRotation(0);
	deadBody.setRotation(0);
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
	if (body.getFillColor() == BOSS_KANGAROO_COLOUR && velo.y == 0)
	{
		velo.y = -body.getSize().y / 3;
	}
	std::cout << body.getPosition().x << " " << body.getPosition().y << std::endl;

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
		else  if (body.getFillColor() == BOSS_KANGAROO_COLOUR && t_blocks.at(index).getFillColor() == ROOF_COLOUR)//roof colision
		{
			if (body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
			{
				body.setPosition(body.getPosition().x, t_blocks.at(index).getPosition().y + t_blocks.at(index).getSize().y);
				velo.y = 0.1;//zero allow jumping when touching roof
			}
		}
	}
}

Boss::Boss()
{
	health = 1;
	velo.x = -MAX_MOVE;
}

void Boss::create(BossData t_bossData)
{
	body.setPosition(t_bossData.m_position);
	body.setSize(t_bossData.m_size);
	deadBody = body;
	deadBody.setSize(sf::Vector2f(body.getSize().x, 0));
	if (t_bossData.m_type == "wall")
	{
		body.setFillColor(BOSS_WALL_COLOUR);
		deadBody.setFillColor(sf::Color(body.getFillColor().r / 2, body.getFillColor().g / 2, body.getFillColor().b / 2));
		health = WALL_HEALTH;
	}
	else if (t_bossData.m_type == "spider")
	{
		body.setFillColor(BOSS_SPIDER_INSIDE_COLOUR);
		deadBody.setFillColor(BOSS_SPIDER_OUTSIDE_COLOUR);
		health = SPIDER_HEALTH;
	}
	else if (t_bossData.m_type == "bird")
	{
		deadBody.setSize(sf::Vector2f(0, body.getSize().y));
		body.setFillColor(BOSS_BIRD_COLOUR);
		wing = body;
		wing.setOrigin(0, wing.getSize().y);
		deadWing = wing;
		deadBody.setFillColor(sf::Color(body.getFillColor().r / 2, body.getFillColor().g / 2, body.getFillColor().b / 2));
		deadWing.setFillColor(deadBody.getFillColor());
		health = BIRD_HEALTH;
	}
	else if (t_bossData.m_type == "kangaroo")
	{
		body.setFillColor(BOSS_KANGAROO_COLOUR);
		deadBody.setFillColor(sf::Color(body.getFillColor().r / 2, body.getFillColor().g / 2, body.getFillColor().b / 2));
		health = KANGAROO_HEALTH;
	}
	else if (t_bossData.m_type == "mutant")
	{
		body.setFillColor(BOSS_MUTANT_COLOUR);
		deadBody.setFillColor(sf::Color::Blue);
		health = MUTANT_HEALTH;
	}
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
		if (body.getFillColor() == BOSS_BIRD_COLOUR && wing.getGlobalBounds().intersects(t_bullets.at(bullies).getGlobalBounds()))
		{
			health--;
			temp = bullies;
		}
	}
	if (body.getFillColor() == BOSS_BIRD_COLOUR)
	{
		flying(t_blocks);
	}
	else if (body.getFillColor() == BOSS_MUTANT_COLOUR)
	{
		teleporting(t_blocks);
	}
	else
	{
		moveMent(t_blocks);
	}

	deadBody.setPosition(body.getPosition());
	if (health >= 0)
	{
		if (body.getFillColor() == BOSS_WALL_COLOUR)
		{
			deadBody.setSize(sf::Vector2f(body.getSize().x, (body.getSize().y / WALL_HEALTH) * (WALL_HEALTH - health)));
		}
		else if (body.getFillColor() == BOSS_SPIDER_INSIDE_COLOUR)
		{
			deadBody.setSize(sf::Vector2f(body.getSize().x, (body.getSize().y / SPIDER_HEALTH) * (SPIDER_HEALTH - health)));
		}
		else if (body.getFillColor() == BOSS_BIRD_COLOUR)
		{
			deadBody.setSize(sf::Vector2f( (body.getSize().x / BIRD_HEALTH) * (BIRD_HEALTH - health), body.getSize().y));
			deadWing.setSize(deadBody.getSize());
			deadWing.setPosition(body.getPosition());
			wing.setPosition(body.getPosition());
		}
		else if (body.getFillColor() == BOSS_KANGAROO_COLOUR)
		{
			deadBody.setSize(sf::Vector2f(body.getSize().x, (body.getSize().y / KANGAROO_HEALTH) * (KANGAROO_HEALTH - health)));
		}
		else if (body.getFillColor() == BOSS_MUTANT_COLOUR)
		{
			deadBody.setSize(sf::Vector2f(body.getSize().x, (body.getSize().y / MUTANT_HEALTH) * (MUTANT_HEALTH - health)));
		}
	}
	return temp;
}

void Boss::render(sf::RenderWindow & t_window)
{
	t_window.draw(body);
	t_window.draw(deadBody);
	if (body.getFillColor() == BOSS_BIRD_COLOUR)
	{
		t_window.draw(wing);
		t_window.draw(deadWing);
	}
}

int Boss::getHealth()
{
	return health;
}

sf::RectangleShape Boss::getBody()
{
	int temp = body.getRotation();
	if ((temp % 2) == 0)
	{
		return body;
	}
	else
	{
		return wing;
	}
}

void Boss::flying(std::vector<sf::RectangleShape> t_blocks)
{
	if (velo.y == 0)
	{
		velo.y = MAX_MOVE;
	}
	if (velo.x < -MAX_MOVE * 8 || velo.x > MAX_MOVE * 8)
	{
		velo.x *= .9;
	}
	else
	{
		velo.x *= 1.5;
	}
	if (velo.y < -MAX_MOVE * 8 || velo.y > MAX_MOVE * 8)
	{
		velo.y *= .9;
	}
	else
	{
		velo.y *= 1.5;
	}

	if (rotateUp)
	{
		body.rotate(-1);
		if (body.getRotation() < 360 -MAX_ROTATION && body.getRotation() > MAX_ROTATION)
		{
			rotateUp = false;
		}
	}
	else
	{
		body.rotate(1);
		if (body.getRotation() < 360 - MAX_ROTATION && body.getRotation() > MAX_ROTATION)
		{
			rotateUp = true;
		}
	}
	deadBody.setRotation(body.getRotation());
	wing.setRotation(180 - body.getRotation());
	deadWing.setRotation(wing.getRotation());

	body.move(velo);
	//wall collision
	for (int index = 0; index < t_blocks.size(); index++)
	{
		if (body.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()) || wing.getGlobalBounds().intersects(t_blocks.at(index).getGlobalBounds()))
		{
			if (t_blocks.at(index).getFillColor() == WALL_COLOUR)
			{
				if (t_blocks.at(index).getPosition().x < body.getPosition().x)
				{
					velo.x = MAX_MOVE;
				}
				else
				{
					velo.x = -MAX_MOVE;
				}
			}
			else if(t_blocks.at(index).getFillColor() == FLOOR_COLOUR)
			{
				velo.y = -MAX_MOVE;
			}
			else if (t_blocks.at(index).getFillColor() == ROOF_COLOUR)
			{
				velo.y = MAX_MOVE;
			}
		}
	}
}

void Boss::teleporting(std::vector<sf::RectangleShape> t_blocks)
{
	timer++;
	if (timer >= TELEPORT_COOLDOWN)
	{
		timer = 0;		
		if (midTeleport)
		{
			body.setPosition(-2000, -1000);
			midTeleport = false;
		}
		else
		{
			switch (rand() % 5)
			{
			case 0:
				body.setPosition(t_blocks.at(t_blocks.size() - 1).getPosition());
				break;
			case 1:
				body.setPosition(t_blocks.at(t_blocks.size() - 2).getPosition());
				break;
			case 2:
				body.setPosition(t_blocks.at(t_blocks.size() - 3).getPosition());
				break;
			case 3:
				body.setPosition(t_blocks.at(t_blocks.size() - 4).getPosition());
				break;
			case 4:
				body.setPosition(t_blocks.at(t_blocks.size() - 5).getPosition());
				break;
			default:
				break;
			}
			body.move(0, -body.getSize().y);
			midTeleport = true;
		}
	}
}
