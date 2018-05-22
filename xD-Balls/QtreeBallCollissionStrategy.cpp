#include "QtreeBallCollissionStrategy.hpp"
#include "Qtree.hpp"


void QtreeBallCollissionStrategy::handle(std::vector<Ball>& balls)
{

	tml::qtree<double, Ball*> tree(bbox.rect.x - 100, bbox.rect.y - 100, bbox.rect.x + bbox.rect.w + 100, bbox.rect.y + bbox.rect.h + 100);
	int fail_cnt = 0;

	
	int balls_n = balls.size();
	// build tree
	for (int i = 0; i < balls_n - 1; i++) {
		if (!tree.add_node(balls[i].position.x, balls[i].position.y, &balls[i])) {
			fail_cnt += 1;
		}
	}
	
	std::vector<const tml::qtree<double, Ball*>::node_type *> result;
	
	// querry tree and handle collisions
	for (int i = 0; i < balls_n - 1; i++) {
		Ball* b1 = &balls[i];

		tree.search(b1->position.x, b1->position.y, b1->r * 2, result);

		// continue if there is no results
		if (result.size() <= 0) continue;

		for (auto node : result) {
			Ball* b2 = node->data;

			float distance = b1->position.distance(b2->position);
			if (distance < b1->r + b2->r) {
				plowBalls(*b1, *b2, distance);
				ballsCollision(*b1, *b2);
			}
		}
	}	
}

QtreeBallCollissionStrategy::QtreeBallCollissionStrategy(const Rectangle bbox) : BallCollissionStrategy("qtree"), bbox(bbox)
{
}

QtreeBallCollissionStrategy::~QtreeBallCollissionStrategy()
{
}

QtreeBallCollissionStrategy::QtreeBallCollissionStrategy(const Rectangle bbox, std::string name) : BallCollissionStrategy(name), bbox(bbox)
{
}



