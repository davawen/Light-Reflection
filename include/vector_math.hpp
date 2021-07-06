#ifndef __VECTOR_MATH_
#define __VECTOR_MATH_

#include <SFML/Graphics.hpp>

float dotProduct(float x1, float y1, float x2, float y2);
float dotProduct(const sf::Vector2f &a, const sf::Vector2f &b);
float dotProduct(const sf::Vector2f &a, float x2, float y2);

float distanceSq(float x1, float y1, float x2, float y2);
float distanceSq(const sf::Vector2f &a, const sf::Vector2f &b);

#endif