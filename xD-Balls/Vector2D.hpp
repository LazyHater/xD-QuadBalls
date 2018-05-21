#pragma once
#include <iostream>
#include <cmath>

class Vector2D {
public:
	float x, y;

	Vector2D();
	Vector2D(const Vector2D &v);
	Vector2D(float x, float y);	
	
	void set(float x, float y);		// just x and y
	void setTryg(float mag, float angle); //magnitude and angle
	void add(Vector2D v);
	float distance(Vector2D v); // calc distance
	static float distance(Vector2D v1, Vector2D v2);
	void swap(Vector2D &v);
	float magnitude();
	void normalize();
	float scalar(Vector2D v);
	void rotate(float alpha);
	float angle(Vector2D v);
	Vector2D negate();

	Vector2D operator!() const;
	Vector2D operator*(const float f) const;
	float operator*(const Vector2D v) const;
	Vector2D operator/(const float f) const;
	Vector2D operator+(const Vector2D v) const;
	Vector2D operator-(const Vector2D v) const;
	Vector2D& operator+=(const Vector2D v);
	Vector2D& operator-=(const Vector2D v);
	Vector2D& operator/=(const float f);
	Vector2D& operator*=(const float f);
	bool operator==(const Vector2D v);
	friend std::ostream & operator <<(std::ostream & s, const Vector2D & v);
};