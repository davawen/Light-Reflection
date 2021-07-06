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


Arc::Arc(float cx, float cy, float length, float angleOffset, float semiMajor, float semiMinor)
{
	this->m_center = sf::Vector2f(cx, cy);
	
	this->m_length = length;
	this->m_angleOffset = angleOffset;
	
	this->m_semiMajor = semiMajor;
	this->m_semiMinor = semiMinor;
	
	this->m_shape = sf::VertexArray(sf::PrimitiveType::LineStrip, 30);
	
	this->calculateVertices();
}

bool Arc::intersects(float x1, float y1, float x2, float y2, float *i_x1, float *i_y1, float *i_x2, float *i_y2)
{
	// Please help me
	
	// float &a = m_semiMajor;
	// float &b = m_semiMinor;
	float &r = m_semiMajor;
	float &cx = m_center.x;
	float &cy = m_center.y;
	
	// First, check if bounding box intersects
	if(std::max(x1, x2) < cx - r ||
	   std::max(y1, y2) < cy - r ||
	   std::min(x1, x2) > cx + r ||
	   std::min(y1, y2) > cy + r   )
	{
		return false;
	}
	
	sf::Vector2f d = sf::Vector2f(x2 - x1, y2 - y1);
	sf::Vector2f f = sf::Vector2f(x1 - cx, y1 - cy);
	
	float a = dotProduct(d, d);
	float b = 2.f * dotProduct(f, d);
	float c = dotProduct(f, f) - r * r;
	
	float discriminant = b * b - 4.f * a * c;
	
	if(discriminant < 0.f) return false;
	else
	{
		// ray didn't totally miss sphere,
		// so there is a solution to
		// the equation.

		discriminant = sqrt(discriminant);

		// either solution may be on or off the ray so need to test both
		// t1 is always the smaller value, because BOTH discriminant and
		// a are nonnegative.
		float t1 = ( -b - discriminant ) / ( 2 * a );
		float t2 = ( -b + discriminant ) / ( 2 * a );

		// 3x HIT cases:
		//          -o->             --|-->  |            |  --|->
		// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

		// 3x MISS cases:
		//       ->  o                     o ->              | -> |
		// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

		if(t1 >= 0 && t1 <= 1)
		{
			// t1 is the intersection, and it's closer than t2
			// (since t1 uses -b - discriminant)
			// Impale, Poke
			
			*i_x1 = x1 + t1 * d.x;
			*i_y1 = y1 + t1 * d.y;
			
			return true;
		}

		// here t1 didn't intersect so we are either started
		// inside the sphere or completely past it
		if(t2 >= 0 && t2 <= 1)
		{
			*i_x1 = x1 + t2 * d.x;
			*i_y1 = y1 + t2 * d.y;
			
			// ExitWound
			return true;
		}

		// no intn: FallShort, Past, CompletelyInside
		return false;
	}
	
	return false;
}

void Arc::calculateVertices()
{
	int numVertices = 30;
	
	// int numVertices = std::max(static_cast<int>( m_length / 30.f ), 2);

	// m_shape = sf::VertexArray(sf::PrimitiveType::LineStrip, numVertices);
	
	for(int i = 0; i < numVertices; i++)
	{
		float angle = static_cast<float>(i) / (numVertices-1) * m_length + m_angleOffset;
		
		m_shape[i].position = sf::Vector2f(m_center.x + cosf(angle)*m_semiMajor, m_center.y + sinf(angle)*m_semiMinor);
	}
}

void Arc::draw(sf::RenderWindow &window)
{
	window.draw(m_shape);
}