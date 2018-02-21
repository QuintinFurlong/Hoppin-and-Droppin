#include"Level.h"

Level::Level()
{
	if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "error";
	}
	for (int i = 0; i < 5; i++)
	{
		m_text[i].setString("level");
		m_text[i].setFont(m_font);
		m_text[i].setCharacterSize(24);
		m_text[i].setPosition(675, i * 100 + 200);
		m_text[i].setFillColor(sf::Color::Blue);
		
		m_buttons[i].setPosition(sf::Vector2f(400, i * 100 + 200));
		m_buttons[i].setSize(sf::Vector2f(600, 60));
		m_buttons[i].setFillColor(sf::Color::White);
		m_buttons[i].setOutlineColor(sf::Color::Blue);
		m_buttons[i].setOutlineThickness(5);
	}
	m_header.setString("Please Select a Level");
	m_header.setFont(m_font);
	m_header.setCharacterSize(36);
	m_header.setPosition(500, 20);
	m_header.setFillColor(sf::Color::Red);
}

void Level::update()
{
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
