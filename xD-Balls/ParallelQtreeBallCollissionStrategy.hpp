#pragma once
#include "BallCollissionStrategy.hpp"
class ParallelQtreeBallCollissionStrategy :
	public BallCollissionStrategy
{
public:
	ParallelQtreeBallCollissionStrategy();
	virtual ~ParallelQtreeBallCollissionStrategy();

	// Inherited via BallCollissionStrategy
	virtual void handle(std::vector<Ball>& balls) override;
};

