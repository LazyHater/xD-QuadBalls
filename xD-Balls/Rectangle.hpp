#pragma once
#include "Object.hpp"
#include "Vector2D.hpp"

class Rectangle : public Object {
public:
	struct rectangle {
		float x, y, w, h;
	} rect;

	Rectangle();
	Rectangle(Vector2D center, float width, float height);
	Rectangle(Vector2D v1, Vector2D v2);
	Rectangle(float x1, float y1, float x2, float y2);

	virtual void move(Vector2D delta);
	void set(Vector2D v1, Vector2D v2);
	void set(Vector2D center, float width, float height);
	virtual void update(float delta_t);
	bool isIn(Vector2D v);
};