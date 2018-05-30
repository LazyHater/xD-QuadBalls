#pragma once
#include "Object.hpp"

class Ball : public Object
{
public:
	static int n;

	double r;
	double m;
	bool collided = false;
	double bounce_factor;
	unsigned int texture_id = 0;

	Ball();
	~Ball();

	explicit Ball(const Ball& ball);
	explicit Ball(double x, double y, sf::Color c = sf::Color::Blue, int texture_id = 0);
	explicit Ball(Vector2D v, sf::Color c = sf::Color::Blue);

	virtual void update(double delta_t);
};