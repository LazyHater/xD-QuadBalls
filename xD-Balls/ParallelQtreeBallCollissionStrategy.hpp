#pragma once
#include "QtreeBallCollissionStrategy.hpp"
#include "ctpl_stl.hpp"
#include "Rectangle.hpp"

class ParallelQtreeBallCollissionStrategy :
	public QtreeBallCollissionStrategy
{
public:
	ParallelQtreeBallCollissionStrategy(const Rectangle bbox);
	virtual ~ParallelQtreeBallCollissionStrategy();

	// Inherited via BallCollissionStrategy
	virtual void handle(std::vector<Ball>& balls) override;

private:
	ctpl::thread_pool* p;
};

