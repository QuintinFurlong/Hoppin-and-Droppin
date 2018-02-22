#include"MainMenu.h"

MainMenu::MainMenu()
{
	//load font
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "error";
	}
	//set font
	m_title.setFont(m_font);
	m_backStory.setFont(m_font);
	m_controls.setFont(m_font);
	m_controlScheme.setFont(m_font);
	m_backStoryDisplay.setFont(m_font);
	m_levelSelect.setFont(m_font);

	//set string
	m_title.setString("Hoppin & Droppin");
	m_backStory.setString("Backstory");
	m_controls.setString("Controls");
	m_controlScheme.setString("Press 'a' to jump /n Use left analogstick to move /n use right analogstick to aim /n Press 'RT' to shoot");
	m_backStoryDisplay.setString("This game is about a guy shooting things up");
	m_levelSelect.setString("Level Select");

	//setPosition
	m_title.setPosition(sf::Vector2f(570, 100));
	m_backStory.setPosition(sf::Vector2f(620, 310));
	m_controls.setPosition(sf::Vector2f(620, 410));
	m_controlScheme.setPosition(sf::Vector2f(20, 20));
	m_backStoryDisplay.setPosition(sf::Vector2f(20, 20));
	m_levelSelect.setPosition(sf::Vector2f(620, 210));

	//setSize
	m_title.setCharacterSize(36);
	m_backStory.setCharacterSize(24);
	m_controls.setCharacterSize(24);
	m_controlScheme.setCharacterSize(24);
	m_backStoryDisplay.setCharacterSize(24);
	m_levelSelect.setCharacterSize(24);

	//set colour
	m_title.setFillColor(sf::Color::Red);
	m_backStory.setFillColor(sf::Color::Blue);
	m_controls.setFillColor(sf::Color::Blue);
	m_controlScheme.setFillColor(sf::Color::Blue);
	m_backStoryDisplay.setFillColor(sf::Color::Blue);
	m_levelSelect.setFillColor(sf::Color::Blue);

	for (int i = 0; i < 3; i++)
	{
		m_buttons[i].setPosition(sf::Vector2f(400, i * 100 + 200));
		m_buttons[i].setSize(sf::Vector2f(600, 60));
		m_buttons[i].setFillColor(sf::Color::White);
		m_buttons[i].setOutlineColor(sf::Color::Blue);
		m_buttons[i].setOutlineThickness(5);
	}

}

void MainMenu::update(sf::Time &t_time)
{
	if (sf::Joystick::isButtonPressed(0, 0))
	{
		//set to game
	}

	if (sf::Joystick::isButtonPressed(0, 1))
	{
		displayControls = true;
	}

	if (sf::Joystick::isButtonPressed(0, 3))
	{
		displayLore = true;
	}
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

	if (count > 2)
	{
		count = 0;
	}
	else if (count < 0)
	{
		count = 2;
	}
	for (int i = 0; i < 3; i++)
	{
		m_buttons[i].setOutlineColor(sf::Color::Blue);
		m_buttons[count].setOutlineColor(sf::Color::Red);
	}
}

void MainMenu::render(sf::RenderWindow & t_window)
{
	if (!displayControls && !displayLore)//if neither y nor x has been pressed
	{
		for (int i = 0; i < 3; i++)
		{
			t_window.draw(m_buttons[i]);
		}

		t_window.draw(m_title);
		t_window.draw(m_controls);
		t_window.draw(m_backStory);
		t_window.draw(m_levelSelect);

	}
	else if (displayControls)//if y has been pressed
	{
		t_window.draw(m_controlScheme);//display controls
	}
	else//if x has been pressed
	{
		t_window.draw(m_backStoryDisplay);//display lore
	}
}
