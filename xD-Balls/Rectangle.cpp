#include "Rectangle.hpp"

Rectangle::Rectangle() = default;
Rectangle::~Rectangle() = default;

Rectangle::Rectangle(const Vector2D center, const double width, const double height) {
	position = center;
	rect.x = position.x - width / 2.0f;
	rect.y = position.y - height / 2.0f;
	rect.w = width;
	rect.h = height;
}

Rectangle::Rectangle(const Vector2D v1, const Vector2D v2) { set(v1, v2); }

Rectangle::Rectangle(const double x1, const double y1, const double x2, const double y2) {
	set(Vector2D(x1, y1), Vector2D(x2, y2));
}

void Rectangle::move(const Vector2D& delta) {
	position += delta;
	rect.x += delta.x;
	rect.y += delta.y;
}

void  Rectangle::set(const Vector2D& v1, const Vector2D& v2) {
	position = (v1 + v2) / 2.0f;
	rect.w = abs(v1.x - v2.x);
	rect.h = abs(v1.y - v2.y);
	rect.x = position.x - rect.w / 2.0f;
	rect.y = position.y - rect.h / 2.0f;
}

void Rectangle::set(const Vector2D& center, const double width, const double height) {
	position = center;
	rect.x = position.x - width / 2.0f;
	rect.y = position.y - height / 2.0f;
	rect.w = width;
	rect.h = height;
}

void Rectangle::update(const double delta_t) {
	//std::cout << "rectangle update";
}

bool Rectangle::isIn(const Vector2D& v) const {
	if ((abs(v.x - position.x) < rect.w / 2)
		&& (abs(v.y - position.y) < rect.h / 2)) return true;
	return false;
}