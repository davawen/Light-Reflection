#include "vector_math.hpp"

float dotProduct(float x1, float y1, float x2, float y2)
{
	return x1 * x2 + y1 * y2;
}

float dotProduct(const sf::Vector2f &a, const sf::Vector2f &b)
{
	return a.x * b.x + a.y * b.y;
}

float dotProduct(const sf::Vector2f &a, float x2, float y2)
{
	return a.x * x2 + a.y * y2;
}

float crossProduct(float x1, float y1, float x2, float y2)
{
	return x1 * y2 - y1 * x2;
}
float crossProduct(const sf::Vector2f &a, const sf::Vector2f &b)
{
	return a.x * b.y - a.y * b.x;
}
float crossProduct(const sf::Vector2f &a, float x2, float y2)
{
	return a.x * y2 - a.y * x2;
}

float distanceSq(float x1, float y1, float x2, float y2)
{
	float diffx = x2 - x1;
	float diffy = y2 - y1;
	
	return diffx * diffx + diffy * diffy;
}

float distanceSq(const sf::Vector2f &a, const sf::Vector2f &b)
{
	float diffx = a.x - b.x;
	float diffy = a.y - b.y;
	
	return diffx * diffx + diffy * diffy;
}