#pragma once
#include "Ball.hpp"

int Ball::n = 0;

Ball::Ball() { n++; }

Ball::Ball(const Ball  &ball) : r(ball.r), bounce_factor(ball.bounce_factor) , m(ball.m) {
	color = ball.color;
	position = ball.position;
	velocity = ball.velocity;
	acceleration = ball.acceleration;
	n++;
}

Ball::Ball(float x, float y, sf::Color c) : r(5) {
	color = c;
	position.set(x, y);
	m = r*r*M_PI;
	n++;
}

Ball::Ball(Vector2D v, sf::Color c) : r(5) {
	color = c;
	position = v;
	m = r*r*M_PI;
	n++;
}

void Ball::update(float delta_t) {
	if (collided) {
		velocity = velocity*bounce_factor;
		collided = false;
	}
	velocity.add(acceleration*delta_t);
	position.add(velocity*delta_t);
}

Ball::~Ball() {
	n--;
}