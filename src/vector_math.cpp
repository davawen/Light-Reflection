#include "vector_math.hpp"

float dotProduct(const sf::Vector2f &a, const sf::Vector2f &b)
{
	return a.x * b.x + a.y * b.y;
}