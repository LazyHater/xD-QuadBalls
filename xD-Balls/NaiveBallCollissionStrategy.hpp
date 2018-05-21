#pragma once
#include "BallCollissionStrategy.hpp"
class NaiveBallCollissionStrategy :
	public BallCollissionStrategy
{
public:
	virtual void handle(std::vector<Ball>& balls);


	NaiveBallCollissionStrategy();
	virtual ~NaiveBallCollissionStrategy();
};

