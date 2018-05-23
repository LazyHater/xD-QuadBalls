#pragma once
#include "BallCollissionStrategy.hpp"

class NaiveBallCollissionStrategy :
	public BallCollissionStrategy
{
public:
	NaiveBallCollissionStrategy();
	virtual ~NaiveBallCollissionStrategy();
	
	virtual void handle(std::vector<Ball>& balls) override;
};

