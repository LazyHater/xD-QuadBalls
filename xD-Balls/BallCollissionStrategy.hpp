#pragma once
#include <vector>

#include "Ball.hpp"

class BallCollissionStrategy
{
public:
	virtual void handle(std::vector<Ball> balls) = 0;
	void ballsCollision(Ball &ball_1, Ball &ball_2);
	void plowBalls(Ball &ball_1, Ball &ball_2, float distance);
	BallCollissionStrategy();
	~BallCollissionStrategy();
};

