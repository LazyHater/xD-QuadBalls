#pragma once
#include <cmath>
#include <vector>

#include "Ball.hpp"
#include "Rectangle.hpp"

class BallSpawner
{
public:
	std::vector<Ball> balls;
	Vector2D position;
	size_t texture_max_id;

	BallSpawner();
	~BallSpawner();

	void deployBalls(const Ball source, int n, bool random_velocity = true, bool random_radius = false, bool random_color = true, size_t texture_max_id = 2);
	void update(double delta_t);
};
