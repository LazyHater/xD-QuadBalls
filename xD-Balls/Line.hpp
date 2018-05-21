#pragma once
#include <cmath>

#include "Rectangle.hpp"

class Line : public Object {
public:
	Vector2D p1, p2;
	Vector2D dirVec, norVec;
	Rectangle collideBox;
	float angle, length, width = 2.0f;

	Line();
	Line(const Line &l);
	Line(Vector2D p1, Vector2D p2, sf::Color color);

	void set(Vector2D p1, Vector2D p2);

	virtual void move(Vector2D delta);
	void update();
	virtual void update(float delta_t);
	float distance(Vector2D p);

};