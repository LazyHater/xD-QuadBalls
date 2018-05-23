#pragma once
#include "ctpl_stl.hpp"

#include "QtreeBallCollissionStrategy.hpp"
#include "Rectangle.hpp"
#include "Qtree.hpp"


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

	static void helper(int id, tml::qtree<double, Ball*>* tree, std::vector<Ball>* balls, int from, int to);
};

