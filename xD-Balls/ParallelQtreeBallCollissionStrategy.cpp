#define _USE_MATH_DEFINES
#include <cmath>
#include "ParallelQtreeBallCollissionStrategy.hpp"
#include  <string>
#include  <sstream>

#include <thread>
#include <chrono>

#include "Rectangle.hpp"
#include "Utils.hpp"

void ParallelQtreeBallCollissionStrategy::helper(int id, tml::qtree<double, Ball*>* tree, std::vector<Ball>* balls, int from, int to) {
	int balls_n = balls->size();
	std::vector<const tml::qtree<double, Ball*>::node_type *> result;

	// querry tree and handle collisions
	for (int i = from; i < to; i++) {
		Ball* b1 = &((*balls)[i]);

		tree->search(b1->position.x, b1->position.y, b1->r * 2, result);

		// continue if there is no results
		if (result.size() <= 0) continue;

		for (auto node : result) {
			Ball* b2 = node->data;

			double distance = b1->position.distance(b2->position);
			if (distance < b1->r + b2->r) {
				plowBalls(*b1, *b2, distance);
				ballsCollision(*b1, *b2);
			}
		}
	}
}

ParallelQtreeBallCollissionStrategy::ParallelQtreeBallCollissionStrategy(const Rectangle bbox) : QtreeBallCollissionStrategy(bbox, "parallel_qtree") {
	this->p = new ctpl::thread_pool;
	this->p->resize(4);
}

void ParallelQtreeBallCollissionStrategy::handle(std::vector<Ball>& balls) {	
	tml::qtree<double, Ball*> tree(bbox.rect.x - 100, bbox.rect.y - 100, bbox.rect.x + bbox.rect.w + 100, bbox.rect.y + bbox.rect.h + 100);
	int fail_cnt = 0;

	int balls_n = balls.size();
	// build tree
	for (int i = 0; i < balls_n - 1; i++) {
		if (!tree.add_node(balls[i].position.x, balls[i].position.y, &balls[i])) {
			fail_cnt += 1;
		}
	}
	
	int chunks = balls_n / 1000;
	if (chunks <= 4) {
		chunks = 4;
	}
	int chunk_size = balls_n / chunks;

	std::vector<std::future<void>> results(chunks);
	for (int i = 0; i < chunks - 1; i++) {
		results[i] = p->push(helper, &tree, &balls, i * chunk_size, (i + 1) * chunk_size);
	}
	results[chunks - 1] = p->push(helper, &tree, &balls, (chunks - 1) * chunk_size, balls_n); // missing bit


	for (int i = 0; i < chunks; i++) {
		results[i].get();
	}
}

ParallelQtreeBallCollissionStrategy::~ParallelQtreeBallCollissionStrategy()
{
	delete this->p;
}