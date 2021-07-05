#include "surface.hpp"

Line::Line(float x1, float y1, float x2, float y2)
{
	this->m_p1 = sf::Vector2f(x1, y1);
	this->m_p2 = sf::Vector2f(x2, y2);
	
	auto diffx = x2-x1;
	auto diffy = y2-y1;
	
	this->m_normal = sf::Vector2f(-diffy, diffx);
	
	this->m_shape[0].position = this->m_p1;
	this->m_shape[1].position = this->m_p2;
}

bool Line::intersects(float ox1, float oy1, float ox2, float oy2, float *i_x, float *i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = m_p2.x - m_p1.x;     s1_y = m_p2.y - m_p1.y;
	s2_x = ox2 - ox1;     s2_y = oy2 - oy1;

	float s, t;
	s = ( -s1_y * ( m_p1.x - ox1 ) + s1_x * ( m_p1.y - oy1 ) ) / ( -s2_x * s1_y + s1_x * s2_y );
	t = ( s2_x * ( m_p1.y - oy1 ) - s2_y * ( m_p1.x - ox1 ) ) / ( -s2_x * s1_y + s1_x * s2_y );

	if(s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		if(i_x != NULL)
		{
			*i_x = m_p1.x + ( t * s1_x );
			*i_y = m_p1.y + ( t * s1_y );
		}
		
		return true;
	}

	return false; // No collision
}

void Line::draw(sf::RenderWindow &window)
{
	window.draw(m_shape, 2, sf::PrimitiveType::LineStrip);
}