#include "Vector2D.hpp"

Vector2D::Vector2D(const Vector2D &v) : x(v.x), y(v.y) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D::Vector2D() : x(0), y(0) {}

void Vector2D::set(float x, float y) {
	this->x = x;
	this->y = y;
}

void Vector2D::setTryg(float mag, float angle) {
	this->set(mag*cos(angle), -mag*sin(angle));
}

void Vector2D::add(Vector2D v) {
	this->set(x + v.x, y + v.y);
}

float Vector2D::distance(Vector2D v) {
	float a = v.x - x;
	float b = v.y - y;
	return sqrt(a*a+b*b);
}

float Vector2D::distance(Vector2D v1, Vector2D v2) {
	float a = v1.x - v2.x;
	float b = v1.y - v2.y;
	return sqrt(a*a + b*b);
}

void Vector2D::swap(Vector2D &v) {
	Vector2D buff(v);
	v = *this;
	*this = buff;
}
float Vector2D::magnitude() {
	return sqrt(x*x+y*y);
}

void  Vector2D::normalize() {
	float mag = sqrt(x*x+y*y);
	this->set(x / (float)mag, y / (float)mag);
}

float Vector2D::scalar(Vector2D v) {
	return (x*v.x + y*v.y);
}

void Vector2D::rotate(float alpha) {
	float tempx=x, tempy=y;
	x = tempx*cos(alpha) - tempy*sin(alpha);
	y = tempx*sin(alpha) + tempy*cos(alpha);
}

Vector2D Vector2D::negate() {
	this->set(-x, -y);
	return *this;
}

float Vector2D::angle(Vector2D v) {
	float scalar = this->scalar(v);
	return acos(scalar / (float)(this->magnitude()*v.magnitude()));
}

Vector2D Vector2D::operator!() const {
	return Vector2D(-x,-y);
}

Vector2D Vector2D::operator*(const float f) const {
	return Vector2D(x*f, y*f);
}

Vector2D Vector2D::operator/(const float f) const {
	return Vector2D(x/f, y/f);
}

float Vector2D::operator*(const Vector2D v) const {
	return (x*v.x + y*v.y);
}

Vector2D Vector2D::operator+(const Vector2D v) const {
	return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D v) const {
	return Vector2D(x - v.x, y - v.y);
}

Vector2D& Vector2D::operator/=(const float f) {
	this->set(x/f, y/f);
	return  *this;
}

Vector2D& Vector2D::operator*=(const float f) {
	this->set(x * f, y * f);
	return  *this;
}

Vector2D& Vector2D::operator+=(const Vector2D v) {
	this->set(x + v.x, y + v.y);
	return  *this;
}

Vector2D& Vector2D::operator-=(const Vector2D v) {
	this->set(x - v.x, y - v.y);
	return  *this;
}

bool Vector2D::operator==(const Vector2D v) {
	if (x != v.x) return false;
	if (y != v.y) return false;
	return true;
}

std::ostream & operator <<(std::ostream & s, const Vector2D & v) {
	return s << '<' << v.x << ',' << v.y << '>';
}