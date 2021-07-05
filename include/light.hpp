#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <math.h>
#include <vector>

#include <SFML/Graphics.hpp>

#include "surface.hpp"
#include "vector_math.hpp"

class Light
{
	private:
	
		sf::VertexArray m_shape;
	
	public:
		sf::Vector2f m_start;
		float m_angle;
		
		Light();
		Light(float x, float y, float angle);
		Light(sf::Vector2f pos, float angle);
		
		void calculateBounce(std::vector<Line> lines);
		
		void draw(sf::RenderWindow &window);
};

#endif