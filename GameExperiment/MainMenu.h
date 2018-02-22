#pragma once
#pragma once
#include"SFML\Graphics.hpp"
#include<iostream>

class MainMenu
{
public:
	MainMenu();//constructor

	void update(sf::Time &t_time); //update everything
	void render(sf::RenderWindow & t_window); //draw everything

	sf::Font m_font;//used to store font for game title

	sf::Text m_title;//text used for game name
	sf::Text m_controls;//text used for access to control info
	sf::Text m_backStory; //text used for access to backstory info
	sf::Text m_controlScheme; //text used for describe controls
	sf::Text m_backStoryDisplay; //used to display lore
	sf::Text m_levelSelect; //text used for level button

	bool displayControls = false; //used to swap between texts
	bool displayLore = false; //used to swap between texts


	sf::RectangleShape m_buttons[3];//buttons for the menu
	int count = 0;
	bool stickMoved = false;
};
