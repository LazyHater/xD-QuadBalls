#pragma once
#include <vector>

#include "Ball.hpp"

class BallCollissionStrategy
{
public:
	BallCollissionStrategy(std::string name);
	~BallCollissionStrategy();

	virtual void handle(std::vector<Ball>& balls) = 0;
	const std::string name;

protected:
	static void ballsCollision(Ball &ball_1, Ball &ball_2);
	static void plowBalls(Ball &ball_1, Ball &ball_2, float distance);

};

