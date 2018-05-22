#include "ParallelQtreeBallCollissionStrategy.hpp"
#include  <string>
#include  <sstream>
//#include <windows.h>.

#include <thread>
#include <chrono>

#include "Rectangle.hpp"
#include "Qtree.hpp"

#include "Utils.hpp"

void ballsCollision(Ball & ball_1, Ball & ball_2)
{
	if (ball_1.m == ball_2.m) {
		ball_1.velocity.swap(ball_2.velocity);
	}
	else {
		float mpm = ball_1.m + ball_2.m;
		Vector2D v1(ball_1.velocity);
		Vector2D v2(ball_2.velocity);

		ball_1.velocity = (v1*(ball_1.m - ball_2.m) + v2 * ball_2.m*2.0f) / mpm; //((m1-m2)*v1+2m2v2)/(m1+m2)
		ball_2.velocity = (v2*(ball_2.m - ball_1.m) + v1 * ball_1.m*2.0f) / mpm; //((m2-m1)*v2+2m1v1)/(m1+m2)
	}

	/*
	if (ball_to_ball_bounce) {
	ball_1.collided = true;
	ball_2.collided = true;
	}
	*/
}

void plowBalls(Ball & ball_1, Ball & ball_2, float distance)
{
	Vector2D delta;
	float how_far = (ball_1.r + ball_2.r - distance);

	if (ball_2.position == ball_1.position) {
		delta.setTryg(1.0, randFromTo(0, M_PI * 2));
	}
	else {
		delta = ball_2.position - ball_1.position;
		delta.normalize();
	}
	float temp = how_far / (ball_1.m + ball_2.m);// d/(m+M)

	ball_1.position -= delta * (temp*ball_2.m);
	ball_2.position += delta * (temp*ball_1.m);
}

void helper(int id,	tml::qtree<double, Ball*>* tree, std::vector<Ball>* balls, int from, int to) {
	
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

			float distance = b1->position.distance(b2->position);
			if (distance < b1->r + b2->r) {
				plowBalls(*b1, *b2, distance);
				ballsCollision(*b1, *b2);
			}
		}
	}
}

void mmm(int id, const std::string & s) {
	std::cout << "function " << id << ' ' << s << '\n';
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

ParallelQtreeBallCollissionStrategy::ParallelQtreeBallCollissionStrategy(const Rectangle bbox) : QtreeBallCollissionStrategy(bbox, "parallel_qtree")
{
	
	this->p = new ctpl::thread_pool;
	this->p->resize(4);
	/*
	ctpl::thread_pool p(4);
	int n = 100;
	printf("scheduling %i tasks\n", n);
	for (int i = 0; i < n; i++) {
		printf("scheduling task %i\n", i);
		std::stringstream ss;
		ss << "worked " << i << "\n";
		p.push(mmm, ss.str());
	}

	printf("scheduling done");
	*/
}

void ParallelQtreeBallCollissionStrategy::handle(std::vector<Ball>& balls)
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