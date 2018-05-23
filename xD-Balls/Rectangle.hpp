#pragma once
#include "Object.hpp"
#include "Vector2D.hpp"

class Rectangle : public Object
{
public:
	struct rectangle {
		double x, y, w, h;
	} rect;

	Rectangle();
	~Rectangle();
	
	explicit Rectangle(const Vector2D center, const double width, const double height);
	explicit Rectangle(const Vector2D v1, const Vector2D v2);
	explicit Rectangle(const double x1, const double y1, const double x2, const double y2);

	void move(const Vector2D& delta);
	void set(const Vector2D& v1, const Vector2D& v2);
	void set(const Vector2D& center, const double width, const double height);
	virtual void update(const double delta_t) override;
	bool isIn(const Vector2D& v) const;
};