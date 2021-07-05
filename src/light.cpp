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

void Light::calculateBounce(std::vector<Line> lines)
{
	sf::Vector2f startRay = m_start;
	sf::Vector2f diff;
	sf::Vector2f endRay;
	
	std::vector<sf::Vertex> shape;
	shape.reserve(2);
	
	shape.push_back(sf::Vertex(startRay));
	
	float distance = 0.f;
	float totalDistance = 0.f;
	
	float cosAngle = cosf(m_angle);
	float sinAngle = sinf(m_angle);
	
	float i_x = 0.f;
	float i_y = 0.f;
	
	while(true)
	{
		distance += 100;
		
		diff = sf::Vector2f(cosAngle * distance, sinAngle * distance);
		endRay = startRay + diff;
		
		for(auto &line : lines)
		{
			// Reflect ray on line
			if(line.intersects(startRay.x, startRay.y, endRay.x, endRay.y, &i_x, &i_y))
			{
				shape.push_back(sf::Vertex(sf::Vector2f(i_x, i_y), sf::Color(0xFFFFFF00 + static_cast<int>( 0xFF * pow(1.005f, -totalDistance) ))));
				
				//https://stackoverflow.com/a/573206/14336328
				sf::Vector2f u = line.m_normal * (dotProduct(diff, line.m_normal) / dotProduct(line.m_normal, line.m_normal));
				sf::Vector2f w = diff - u;
				
				sf::Vector2f reflected = w - u;
				
				cosAngle = reflected.x / distance;
				sinAngle = reflected.y / distance;
				
				totalDistance += distance;
				
				distance = 0;
				
				//Avoid infinite collision
				startRay.x = i_x + cosAngle;
				startRay.y = i_y + sinAngle;
				
				// Break out of for loop
				break;
			}
			else
			{
				// Don't step ray too much
				if(distance > 1000)
				{
					totalDistance += 1000;
					
					shape.push_back(sf::Vertex(endRay, sf::Color(0xFFFFFF00 + static_cast<int>( 0xFF*pow(1.005f, -totalDistance )))));
					goto endLoop;
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