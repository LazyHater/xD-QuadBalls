#pragma once
#include "BallCollissionStrategy.hpp"
#include "Rectangle.hpp"

class QtreeBallCollissionStrategy :
	public BallCollissionStrategy
{

public:
	virtual void handle(std::vector<Ball>& balls);

	QtreeBallCollissionStrategy(const Rectangle bbox);
	virtual ~QtreeBallCollissionStrategy();

protected:
	const Rectangle bbox;

	QtreeBallCollissionStrategy(const Rectangle bbox, std::string name);
};

