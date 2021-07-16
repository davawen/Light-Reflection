#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <math.h>
#include <vector>

#include <SFML/Graphics.hpp>

#include "surface.hpp"
#include "vector_math.hpp"

#define M_MAXIMUM_REFLECTIONS 10

class Light
{
	private:
		sf::VertexArray m_shape;
		
		void reflect(const sf::Vector2f &normal, sf::Vector2f &diff, float &distance, float &cosAngle, float &sinAngle);
		
	public:
		sf::Vector2f m_start;
		float m_angle;
		
		Light();
		Light(float x, float y, float angle);
		Light(sf::Vector2f pos, float angle);
		
		void calculateBounce(std::vector<Line> lines, std::vector<Arc> arcs);
		
		void draw(sf::RenderWindow &window);
};

#endif