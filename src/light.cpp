#include "light.hpp"

Light::Light() : Light::Light(0.f, 0.f, 0.f) {}

Light::Light(float x, float y, float angle) :
	Light::Light(sf::Vector2f(x, y), angle) {}

Light::Light(sf::Vector2f pos, float angle)
{
	this->m_start = pos;
	this->m_angle = angle;
	
	this->m_shape = sf::VertexArray(sf::PrimitiveType::LineStrip, 2UL);
}

void Light::reflect(const sf::Vector2f &normal, sf::Vector2f &diff, float &distance, float &cosAngle, float &sinAngle)
{
	//https://stackoverflow.com/a/573206/14336328
	sf::Vector2f u = normal * ( dotProduct(diff, normal) / dotProduct(normal, normal) );
	sf::Vector2f w = diff - u;
	
	sf::Vector2f reflected = w - u;

	cosAngle = reflected.x / distance;
	sinAngle = reflected.y / distance;
}

void Light::calculateBounce(std::vector<Line> lines, std::vector<Arc> arcs)
{
	sf::Vector2f startRay = m_start;
	sf::Vector2f diff;
	sf::Vector2f endRay;
	
	std::vector<sf::Vertex> shape;
	shape.reserve(2);
	
	shape.push_back(sf::Vertex(startRay, sf::Color(0xFFFF00FF)));
	
	float distance = 0.f;
	float totalDistance = 0.f;
	
	float cosAngle = cosf(m_angle);
	float sinAngle = sinf(m_angle);
	
	float i_x1 = 0.f;
	float i_y1 = 0.f;
	float i_x2 = 0.f;
	float i_y2 = 0.f;
	
	u_int8_t reflections = M_MAXIMUM_REFLECTIONS;
	
	while(reflections > 0)
	{
		distance += 10.f;
		
		diff = sf::Vector2f(cosAngle * distance, sinAngle * distance);
		endRay = startRay + diff;
		
		for(auto &line : lines)
		{
			// Reflect ray on line
			if(line.intersects(startRay.x, startRay.y, endRay.x, endRay.y, &i_x1, &i_y1))
			{
				shape.push_back(sf::Vertex(sf::Vector2f(i_x1, i_y1), sf::Color(0xFFFF00FF)));
				
				this->reflect(line.m_normal, diff, distance, cosAngle, sinAngle);
				
				//Avance ray a bit to avoid infinite collision
				startRay.x = i_x1 + cosAngle;
				startRay.y = i_y1 + sinAngle;
				
				totalDistance += distance;
				
				distance = 0.;
				
				reflections--;
				
				// Break out of for loop
				break;
			}
		}
		
		// Don't step ray too much
		if(distance > 1000.f)
		{
			totalDistance += 1000.f;

			shape.push_back(sf::Vertex(endRay, sf::Color(0xFFFF00FF)));
			break;
		}
		// Don't check arcs if we already reflected on a line
		else if(distance > 0.f)
		{
			for(auto &arc : arcs)
			{
				if(arc.intersects(startRay.x, startRay.y, endRay.x, endRay.y, &i_x1, &i_y1, &i_x2, &i_y2))
				{
					// Find closer intersection point
					// bool isIntersection1Closer = distanceSq(startRay.x, startRay.y, i_x1, i_y1) < distanceSq(startRay.x, startRay.y, i_x2, i_y2);
					
					// float closerX = isIntersection1Closer ? i_x1 : i_x2;
					// float closerY = isIntersection1Closer ? i_y1 : i_y2;
					
					// shape.push_back(sf::Vertex(sf::Vector2f(closerX, closerY), sf::Color(0xFFFF00FF)));
					shape.push_back(sf::Vertex(sf::Vector2f(i_x1, i_y1), sf::Color(0xFFFF00FF)));
					
					
					// The normal is a vector from the center to the intersection point
					this->reflect(sf::Vector2f( i_x1 - arc.m_center.x, i_y1 - arc.m_center.y ), diff, distance, cosAngle, sinAngle);

					//Avance ray a bit to avoid infinite collision
					startRay.x = i_x1 + cosAngle;
					startRay.y = i_y1 + sinAngle;

					totalDistance += distance;
					
					distance = 0.;

					reflections--;
					
					break;
					
					// goto endLoop;
				}
			}
		}
		
	}
	
endLoop:

	int numberVertices = shape.size();
	m_shape = sf::VertexArray(sf::PrimitiveType::LineStrip, numberVertices);
	
	for(int i = 0; i < numberVertices; i++)
	{
		m_shape[i] = shape[i];
	}
}

void Light::draw(sf::RenderWindow &window)
{
	window.draw(m_shape);
}