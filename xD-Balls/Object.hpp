#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"

#define P(x) std::cout<<#x<<": "<<x<<" "
#define D(x) std::cout<<#x<<": "<<x<<" "; std::cout.flush();

#define M_PI 3.14159265358979323846

class Object {
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	//virtual void draw(SDL_Renderer *renderer) = 0;
	//virtual void move(Vector2D delta) = 0;
	virtual void update(float delta_t) = 0;

	sf::Color color;
};