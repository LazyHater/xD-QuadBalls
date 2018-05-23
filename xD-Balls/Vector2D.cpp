#include "Vector2D.hpp"

Vector2D::Vector2D(const Vector2D &v) : x(v.x), y(v.y) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D::~Vector2D() = default;

Vector2D::Vector2D() : x(0), y(0) {}

void Vector2D::set(double x, double y) {
	this->x = x;
	this->y = y;
}

void Vector2D::setTryg(double mag, double angle) {
	this->set(mag*cos(angle), -mag*sin(angle));
}

void Vector2D::add(Vector2D v) {
	this->set(x + v.x, y + v.y);
}

double Vector2D::distance(Vector2D v) {
	double a = v.x - x;
	double b = v.y - y;
	return sqrt(a*a+b*b);
}

double Vector2D::distance(Vector2D v1, Vector2D v2) {
	double a = v1.x - v2.x;
	double b = v1.y - v2.y;
	return sqrt(a*a + b*b);
}

void Vector2D::swap(Vector2D &v) {
	Vector2D buff(v);
	v = *this;
	*this = buff;
}
double Vector2D::magnitude() {
	return sqrt(x*x+y*y);
}

Vector2D& Vector2D::normalize() {
	double mag = sqrt(x*x+y*y);
	this->set(x / (double)mag, y / (double)mag);
	return *this;
}

double Vector2D::scalar(Vector2D v) {
	return (x*v.x + y*v.y);
}

void Vector2D::rotate(double alpha) {
	double tempx=x, tempy=y;
	x = tempx*cos(alpha) - tempy*sin(alpha);
	y = tempx*sin(alpha) + tempy*cos(alpha);
}

Vector2D Vector2D::negate() {
	this->set(-x, -y);
	return *this;
}

double Vector2D::angle(Vector2D v) {
	double scalar = this->scalar(v);
	return acos(scalar / (double)(this->magnitude()*v.magnitude()));
}

Vector2D Vector2D::operator!() const {
	return Vector2D(-x,-y);
}

Vector2D Vector2D::operator*(const double f) const {
	return Vector2D(x*f, y*f);
}

Vector2D Vector2D::operator/(const double f) const {
	return Vector2D(x/f, y/f);
}

double Vector2D::operator*(const Vector2D v) const {
	return (x*v.x + y*v.y);
}

Vector2D Vector2D::operator+(const Vector2D v) const {
	return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D v) const {
	return Vector2D(x - v.x, y - v.y);
}

Vector2D& Vector2D::operator/=(const double f) {
	this->set(x/f, y/f);
	return  *this;
}

Vector2D& Vector2D::operator*=(const double f) {
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