#include "Level.h"
#include "Colour.h"

Level::Level()
{
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "error";
	}
	for (int i = 0; i < 5; i++)
	{
		m_text[i].setFont(m_font);
		m_text[i].setCharacterSize(24);
		m_text[i].setPosition(675, i * 100 + 200);
		m_text[i].setFillColor(MENU_TEXT);

		m_buttons[i].setPosition(sf::Vector2f(400, i * 100 + 200));
		m_buttons[i].setSize(sf::Vector2f(600, 60));
		m_buttons[i].setFillColor(MENU_BUTTON_FILL);
		m_buttons[i].setOutlineColor(MENU_OUTLINE_OFF);
		m_buttons[i].setOutlineThickness(15);
	}

	m_text[0].setString("Level 1");
	m_text[1].setString("Level 2");
	m_text[2].setString("Level 3");
	m_text[3].setString("Level 4");
	m_text[4].setString("Level 5");

	m_header.setString("Please Select a Level");
	m_header.setFont(m_font);
	m_header.setCharacterSize(36);
	m_header.setPosition(500, 20);
	m_header.setFillColor(MENU_TEXT);

	pressReset = false;
}

void Level::update(sf::Time t_time, GameState &gamestate, int &level)
{
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 20)
	{
		if (stickMoved)
		{
			m_buttons[count].setOutlineColor(sf::Color::Blue);
			count++;
			stickMoved = false;
		}
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < -20)
	{
		if (stickMoved)
		{
			m_buttons[count].setOutlineColor(sf::Color::Blue);
			count--;
			stickMoved = false;
		}
	}
	else
	{
		stickMoved = true;
	}

	if (count > 4)
	{
		count = 0;
	}
	else if (count < 0)
	{
		count = 4;
	}
	for (int i = 0; i < 5; i++)
	{
		m_buttons[i].setOutlineColor(MENU_OUTLINE_OFF);
		m_buttons[count].setOutlineColor(MENU_OUTLINE_ON);
	}

	if (sf::Joystick::isButtonPressed(0, 1))
	{
		gamestate = GameState::MainMenu;
	}
	if (sf::Joystick::isButtonPressed(0, 0) && pressReset)
	{
		level = count +1;
		gamestate = GameState::GamePlay;
		pressReset = false;
	}
	else if (!sf::Joystick::isButtonPressed(0, 0))
	{
		pressReset = true;
	}
}

void Level::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < 5; i++)
	{
		t_window.draw(m_buttons[i]);
		t_window.draw(m_text[i]);
	}
	t_window.draw(m_header);
}
