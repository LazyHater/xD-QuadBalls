#pragma once
#include <cmath>

#include "Rectangle.hpp"

class Line : public Object
{
public:
	Vector2D p1, p2;
	Vector2D dirVec, norVec;
	Rectangle collideBox;
	double angle;
	double length = 0;
	double width = 2.0f;

	Line();
	~Line();
	explicit Line(const Line &l);
	explicit Line(const Vector2D p1, const Vector2D p2, const sf::Color color);

	void set(const Vector2D& p1, const Vector2D& p2);

	void move(const Vector2D& delta);
	virtual void update(const double delta_t) override;
	void updateConstants();
	double distance(const Vector2D& v) const;
};