#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <math.h>

#include <SFML/Graphics.hpp>

class Line
{
	private:
		sf::Vertex m_shape[2];
		
	public:
		sf::Vector2f m_p1;
		sf::Vector2f m_p2;
		sf::Vector2f m_normal;
	
		Line(float x1, float y1, float x2, float y2);
		
		bool intersects(float ox1, float oy1, float ox2, float oy2, float *i_x, float *i_y);
		
		void draw(sf::RenderWindow &window);
};

#endif