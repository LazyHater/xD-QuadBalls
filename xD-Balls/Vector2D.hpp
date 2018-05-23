#pragma once
#include <iostream>
#include <cmath>

class Vector2D
{
public:
	double x, y;

	Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(double x, double y);
	~Vector2D();
	
	void set(double x, double y);
	void setTryg(double mag, double angle);
	void add(Vector2D v);
	double distance(Vector2D v);
	static double distance(Vector2D v1, Vector2D v2);
	void swap(Vector2D &v);
	double magnitude();
	void normalize();
	double scalar(Vector2D v);
	void rotate(double alpha);
	double angle(Vector2D v);
	Vector2D negate();

	Vector2D operator!() const;
	Vector2D operator*(const double f) const;
	double operator*(const Vector2D v) const;
	Vector2D operator/(const double f) const;
	Vector2D operator+(const Vector2D v) const;
	Vector2D operator-(const Vector2D v) const;
	Vector2D& operator+=(const Vector2D v);
	Vector2D& operator-=(const Vector2D v);
	Vector2D& operator/=(const double f);
	Vector2D& operator*=(const double f);
	bool operator==(const Vector2D v);
	friend std::ostream & operator <<(std::ostream & s, const Vector2D & v);
};