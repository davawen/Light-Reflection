#include <math.h>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "light.hpp"
#include "surface.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Light reflection!!! UwU");
	
	struct Lamp
	{
		sf::Vector2f m_pos;
		float m_offset;
		Light m_lights[10];
		
		Lamp(float x, float y)
		{
			m_pos.x = x;
			m_pos.y = y;
			
			m_offset = 0.f;
			
			for(int i = 0; i < 10; i++)
			{
				m_lights[i] = Light(x, y, i / 10.f * M_PI * 2.f);
			}
		}
		
		void update(std::vector<Line> &lines)
		{
			for(auto &light : m_lights)
			{
				light.calculateBounce(lines);
				light.m_start = m_pos;
				light.m_angle += m_offset;
			}
			
			m_offset = 0.f;
		}
		
	} lamp{200.f, 200.f};
	
	std::vector<Line> lines =
	{
		Line(50.f, 50.f, 750.f, 150.f),
		Line(650.f, 400.f, 600.f, 800.f)
	};
	
	sf::Clock deltaClock;
	
	float deltaTime;
	
	lamp.update(lines);
	
	bool isUpdatingLamp = false;
	
	while(window.isOpen())
	{
		deltaTime = deltaClock.restart().asSeconds();
		
		auto mousePosition = sf::Mouse::getPosition(window);
		
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					switch(event.mouseButton.button)
					{
						case sf::Mouse::Left:
							isUpdatingLamp = true;
							break;
					}
					break;
				case sf::Event::MouseButtonReleased:
					isUpdatingLamp = false;
					break;
				case sf::Event::MouseWheelScrolled:
					lamp.m_offset = event.mouseWheelScroll.delta * 0.01f;
					
					lamp.update(lines);
					break;
				default:
					break;
			}
		}

		window.clear();
		
		if(isUpdatingLamp)
		{
			lamp.m_pos = sf::Vector2f(mousePosition);
			lamp.update(lines);
		}
		
		for(auto &light : lamp.m_lights)
		{
			light.draw(window);
		}
		
		for(auto &line : lines)
		{
			line.draw(window);
		}
		
		window.display();

		std::cout.flush();
	}

	return 0;
}