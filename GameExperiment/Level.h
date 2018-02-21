#pragma once
#include"SFML\Graphics.hpp"
#include<iostream>

class Level
{
public:
	Level();
	void update();
	void render(sf::RenderWindow & t_window);

private:
	sf::RectangleShape m_buttons[5]; //buttons for level selection
	sf::Font m_font; //font used for text obj
	sf::Text m_header; //text used for title
	sf::Text m_text[5]; //texts used for buttons
};

