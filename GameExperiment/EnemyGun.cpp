#include "EnemyGun.h"

EnemyGun::EnemyGun()//hi
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
	timer = 0;
}

sf::Vector2f EnemyGun::update(sf::RectangleShape t_playerPos, sf::RectangleShape t_enemy, bool t_enemyAlive)
{
	sf::Vector2f temp(0,0);
	if (t_enemyAlive && sqrt(pow(t_playerPos.getPosition().x - t_enemy.getPosition().x, 2) + pow(t_playerPos.getPosition().y - t_enemy.getPosition().y, 2)) < 1000)
	{
		if (timer == 0)
		{
			temp = t_playerPos.getPosition() - t_enemy.getPosition();

			temp = sf::Vector2f(temp.x / sqrt(temp.x * temp.x + temp.y * temp.y), temp.y / sqrt(temp.x * temp.x + temp.y * temp.y));
			bulletVelo[currentBullet] = temp * 20.0f;
			temp = bulletVelo[currentBullet];
			bullet.at(currentBullet).setPosition(t_enemy.getPosition() + t_enemy.getSize() / 2.0f);
			currentBullet++;
			if (currentBullet >= CLIP_SIZE)
			{
				currentBullet = 0;
			}
			bulletVelo[currentBullet] = sf::Vector2f(0, 0);
			timer = FIRE_RATE;
		}
		else
		{
			timer--;
		}
	}

	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo[index] != sf::Vector2f(0, 0))
		{
			bullet.at(index).setPosition(bullet.at(index).getPosition() + bulletVelo[index]);
		}
	}

	return temp;
}

bool EnemyGun::hit(sf::RectangleShape t_player)
{
	bool hit = false;
	for (int i = 0;i < CLIP_SIZE;i++)
	{
		if ( bullet.at(i).getGlobalBounds().intersects(t_player.getGlobalBounds()))
		{
			
			hit = true;
			if (currentBullet != 0)
			{
				bullet.at(currentBullet - 1).setPosition(-1000, -1000);
				bulletVelo.at(currentBullet - 1) = sf::Vector2f(0, 0);
			}
			else
			{
				bullet.at(currentBullet - 1 + CLIP_SIZE).setPosition(-1000, -1000);
				bulletVelo.at(currentBullet - 1 + CLIP_SIZE) = sf::Vector2f(0, 0);
			}
		}
	}

	return hit;
}

void EnemyGun::render(sf::RenderWindow & t_window)
{
	for (int index = 0; index < CLIP_SIZE; index++)
	{
		if (bulletVelo.at(index) != sf::Vector2f(0,0))
		{
			t_window.draw(bullet.at(index));
		}
	}
}
