#define _USE_MATH_DEFINES
#include <cmath>
#include "BallCollissionStrategy.hpp"
#include "Utils.hpp"


void BallCollissionStrategy::ballsCollision(Ball & ball_1, Ball & ball_2)
{
	if (ball_1.m == ball_2.m) {
		ball_1.velocity.swap(ball_2.velocity);
	}
	else {
		double mpm = ball_1.m + ball_2.m;
		Vector2D v1(ball_1.velocity);
		Vector2D v2(ball_2.velocity);

		ball_1.velocity = (v1*(ball_1.m - ball_2.m) + v2 * ball_2.m*2.0f) / mpm; //((m1-m2)*v1+2m2v2)/(m1+m2)
		ball_2.velocity = (v2*(ball_2.m - ball_1.m) + v1 * ball_1.m*2.0f) / mpm; //((m2-m1)*v2+2m1v1)/(m1+m2)
	}

	/*
	if (ball_to_ball_bounce) {
		ball_1.collided = true;
		ball_2.collided = true;
	}
	*/
}

void BallCollissionStrategy::plowBalls(Ball & ball_1, Ball & ball_2, double distance)
{
	Vector2D delta;
	double how_far = (ball_1.r + ball_2.r - distance);

	if (ball_2.position == ball_1.position) {	
		delta.setTryg(1.0, Utils::dRand(0, M_PI * 2));
	}
	else {
		delta = ball_2.position - ball_1.position;
		delta.normalize();
	}
	double temp = how_far / (ball_1.m + ball_2.m);// d/(m+M)

	ball_1.position -= delta * (temp*ball_2.m);
	ball_2.position += delta * (temp*ball_1.m);
}


BallCollissionStrategy::BallCollissionStrategy(std::string name) : name(name)
{
}

BallCollissionStrategy::~BallCollissionStrategy() = default;