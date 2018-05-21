#pragma once
#include <cmath>
#include <vector>

#include "Ball.hpp"
#include "Rectangle.hpp"

class BallSpawner {
public:
	std::vector<Ball> balls;
	Vector2D position;
	int texture_max_id;

	bool ball_to_ball_bounce = false;

	void deployBalls(const Ball source, int n, bool random_velocity = true, bool random_radius = false, bool random_color = true, int texture_max_id = 2);

	void update(float delta_t);
};
