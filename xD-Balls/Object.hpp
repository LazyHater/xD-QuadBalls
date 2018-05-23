#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"

class Object
{
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	virtual void update(const double delta_t) = 0;

	sf::Color color;
};