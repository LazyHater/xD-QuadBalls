#define _USE_MATH_DEFINES
#include <cmath>

#include"Line.hpp"

Line::Line() = default;

Line::~Line() = default;

Line::Line(const Line &l) : p1(l.p1), p2(l.p2), dirVec(l.dirVec), norVec(l.norVec),
collideBox(l.collideBox), angle(l.angle), length(l.length),
width(l.width) {
	color = l.color;
}

Line::Line(const Vector2D p1, const Vector2D p2, const sf::Color color) : p1(p1), p2(p2) {
	this->color = color;
	this->updateConstants();
}

void Line::move(const Vector2D& delta) {
	p1 += delta;
	p2 += delta;
}

void Line::set(const Vector2D& p1, const Vector2D& p2) {
	this->p1 = p1;
	this->p2 = p2;
	this->updateConstants();
};

void Line::updateConstants() {
	double dx = (p1.x - p2.x);
	double dy = (p1.y - p2.y);

	if (dx == 0) {
		collideBox = Rectangle(Vector2D(0, p1.y), Vector2D(5000, p2.y));
	}
	else if (dy == 0) {
		collideBox = Rectangle(Vector2D(p1.x, 0), Vector2D(p2.x, 5000));
	}
	else {
		collideBox = Rectangle(p1, p2);
	}

	dirVec.set(dx, dy);
	length = dirVec.magnitude();
	norVec = dirVec;
	norVec.rotate(M_PI / 2);
	norVec.normalize();
	Vector2D temp = p2 - p1;
	angle = atan2f(static_cast<float>(temp.y), static_cast<float>(temp.x));
	if (norVec.angle(Vector2D(0, -1)) > M_PI / 2.0) {
		norVec.negate();
	}
}



void Line::update(const double delta_t) {
	//std::cout << "line update";
}

double Line::distance(const Vector2D& v) const { // not sure why it works
	double r = (((v.x - p1.x)*(p2.x - p1.x)) + ((v.y - p1.y)*(p2.y - p1.y))) / (length*length);
	double closestX = p1.x + r * (p2.x - p1.x);
	double closestY = p1.y + r * (p2.y - p1.y);
	double distToPointX = closestX - v.x;
	double distToPointY = closestY - v.y;
	double distToPoint = sqrt(distToPointX*distToPointX + distToPointY * distToPointY);
	return distToPoint;
}