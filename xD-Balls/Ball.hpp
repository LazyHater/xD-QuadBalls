#pragma once
#include "Object.hpp"

class Ball : public Object {
public:
	static int n;

	float r;
	float m;
	bool collided = false;

	float bounce_factor;

	Ball();
	Ball(const Ball  &ball);
	Ball(float x, float y, sf::Color c = sf::Color::Blue);
	Ball(Vector2D v, sf::Color c = sf::Color::Blue);
	~Ball();

	virtual void update(float delta_t);
};