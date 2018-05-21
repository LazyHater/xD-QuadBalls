#pragma once
#include "BallCollissionStrategy.hpp"
class DisabledBallCollissionStrategy :
	public BallCollissionStrategy
{
public:
	DisabledBallCollissionStrategy();
	virtual ~DisabledBallCollissionStrategy();

	// Inherited via BallCollissionStrategy
	virtual void handle(std::vector<Ball>& balls) override;
};

