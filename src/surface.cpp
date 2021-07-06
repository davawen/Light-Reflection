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
	
	float A = y2 - y1;
	float B = x1 - x2;
	float C = x2 * y1 - x1 * y2;
	float a = A*A + B*B;
	float b, c;
	
	bool bnz = true;
	if(abs(B) > 0) 
	{
		b = 2.f * ( A * C + A * B * cy - B*B * cx );
		c = C*C + 2.f * B * C * cy - B*B * ( r*r - cx*cx - cy*cy );
	}
	else
	{
		b = 2.f * ( B * C + A * B * cx - A*A * cy );
		c = C*C + 2.f * A * C * cx - A*A * ( r*r - cx*cx - cy*cy );
		bnz = false;
	}
	
	float d = b*b - 4.f * a * c; // discriminant
	if(d < 0.f)
	{
		return false;
	}

	// checks whether a point is within a segment
	auto within = [x1, y1, x2, y2](float x, float y)
	{
		auto d1 = sqrtf(distanceSq(x1, y1, x2, y2));  // distance between end-points
		auto d2 = sqrtf(distanceSq(x1, y1, x, y));    // distance from point to one end
		auto d3 = sqrtf(distanceSq(x2, y2, x, y));    // distance from point to other end
		auto delta = d1 - d2 - d3;
		
		return abs(delta) < 1e-14;                    // true if delta is less than a small tolerance
	};

	auto fx = [A, B, C](float x)
	{
		return -( A * x + C ) / B;
	};

	auto fy = [A, B, C](float y)
	{
		return -( B * y + C ) / A;
	};

	float x, y;
	if(d == 0.f)
	{
		// line is tangent to circle, so just one intersect at most
		if(bnz)
		{
			x = -b / ( 2 * a );
			y = fx(x);
		}
		else
		{
			y = -b / ( 2 * a );
			x = fy(y);
		}

		if(within(x, y))
		{
			*i_x1 = x;
			*i_y1 = y;
			*i_x2 = x;
			*i_y2 = y;
		}
	}
	else 
	{
		// two intersects at most
		d = sqrtf(d);
		if(bnz)
		{
			x = ( -b + d ) / ( 2 * a );
			y = fx(x);
			
			if(within(x, y))
			{
				*i_x1 = x;
				*i_y1 = y;
			}
			
			x = ( -b - d ) / ( 2 * a );
			y = fx(x);

			if(within(x, y))
			{
				*i_x2 = x;
				*i_y2 = y;
			}
		}
		else
		{
			y = ( -b + d ) / ( 2 * a );
			x = fy(y);
			
			if(within(x, y))
			{
				*i_x1 = x;
				*i_y1 = y;
			}
			
			y = ( -b - d ) / ( 2 * a );
			x = fy(y);

			if(within(x, y))
			{
				*i_x2 = x;
				*i_y2 = y;
			}
		}
	}

	return true;
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