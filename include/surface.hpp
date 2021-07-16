#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <math.h>

#include <SFML/Graphics.hpp>

#include "vector_math.hpp"

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

class Arc
{
	private:
		sf::VertexArray m_shape;
	
		sf::Vector2f m_arcStart;
		sf::Vector2f m_arcEnd;
	
	public:
	
		sf::Vector2f m_center;
		/** Number from 0 to 2PI determining the "amount of circle" in the arc */
		float m_length;
		float m_angleOffset;
		
		float m_radius;
		
		Arc(float cx, float cy, float length, float angleOffset, float radius);

		/** Checks if a point is in the arc section or not */
		bool pointInSection(const float &x, const float &y);
		
		bool intersects(float x1, float y1, float x2, float y2, float *i_x1, float *i_y1, float *i_x2, float *i_y2);
		
		void calculateVertices();
		void draw(sf::RenderWindow &window);
};

#endif