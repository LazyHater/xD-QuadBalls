#pragma once
#include "BallCollissionStrategy.hpp"
#include "Rectangle.hpp"

class QtreeBallCollissionStrategy :
	public BallCollissionStrategy
{
	const Rectangle bbox;

public:
	virtual void handle(std::vector<Ball>& balls);

	QtreeBallCollissionStrategy(const Rectangle bbox);
	virtual ~QtreeBallCollissionStrategy();
};

