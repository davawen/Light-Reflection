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
		Light m_lights[12];
		
		Lamp(float x, float y)
		{
			m_pos.x = x;
			m_pos.y = y;
			
			m_offset = 0.f;
			
			for(int i = 0; i < 12; i++)
			{
				m_lights[i] = Light(x, y, i / 12.f * M_PI * 2.f);
			}
		}
		
		void update()
		{
			for(auto &light : m_lights)
			{
				light.m_start = m_pos;
				light.m_angle += m_offset;
			}
			
			m_offset = 0.f;
		}
		
	} lamp{200.f, 200.f};
	
	std::vector<Line> lines =
	{
		// Line(50.f, 50.f, 750.f, 150.f),
		// Line(650.f, 400.f, 600.f, 800.f),
		// Line(500.f, 500.f, 550.f, 550.f)
	};
	
	std::vector<Arc> arcs = 
	{
		Arc(400.f, 400.f, M_PI, -M_PI * .5f, 300.f)
	};
	
	for(auto &arc : arcs) { arc.calculateVertices(); }
	
	sf::Clock deltaClock;
	
	float deltaTime;
	
	lamp.update();
	
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
						default:
							break;
					}
					break;
				case sf::Event::MouseButtonReleased:
					isUpdatingLamp = false;
					break;
				case sf::Event::MouseWheelScrolled:
					lamp.m_offset = event.mouseWheelScroll.delta * 0.01f;
					
					lamp.update();
					break;
				default:
					break;
			}
		}

		window.clear();
		
		if(isUpdatingLamp)
		{
			lamp.m_pos = sf::Vector2f(mousePosition);
			lamp.update();
		}
		
		for(auto &light : lamp.m_lights)
		{
			light.calculateBounce(lines, arcs);
			light.draw(window);
		}
		
		for(auto &line : lines)          { line.draw(window); }
		for(auto &arc : arcs)            { arc.draw(window); }
		
		window.display();
		
		printf("\x1b[1000D%f FPS", 1.f / deltaTime);
		
		std::cout.flush();
	}

	return 0;
}