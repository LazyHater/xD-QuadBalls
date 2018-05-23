#pragma once
#include "BallCollissionStrategy.hpp"
class DisabledBallCollissionStrategy :
	public BallCollissionStrategy
{
public:
	DisabledBallCollissionStrategy();
	virtual ~DisabledBallCollissionStrategy();

	virtual void handle(std::vector<Ball>& balls) override;
};

