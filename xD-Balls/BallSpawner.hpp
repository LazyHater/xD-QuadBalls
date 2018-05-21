#pragma once
#include <cmath>
#include <vector>

#include "Ball.hpp"
#include "Rectangle.hpp"

class BallSpawner {
public:
	std::vector<Ball> balls;
	Vector2D position;

	bool ball_to_ball_bounce = false;

	void handleCollisionBallToBall();
	void handleCollisionBallToBallQuadTree(const Rectangle& box);
	void plowBalls(Ball &ball_1, Ball &ball_2, float distance);
	void ballsCollision(Ball &ball_1, Ball &ball_2);
	void deployBalls(const Ball source, int n, bool random_velocity = true, bool random_radius = false, bool random_color = true);

	void update(float delta_t);
};
