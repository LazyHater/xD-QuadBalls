#include "NaiveBallCollissionStrategy.hpp"



void NaiveBallCollissionStrategy::handle(std::vector<Ball>& balls)
{
	int temp = balls.size();
	for (int i = 0; i < temp - 1; i++) {
		for (int j = i + 1; j < temp; j++) {
			double distance = balls[i].position.distance(balls[j].position);
			if (distance < balls[i].r + balls[j].r) {
				plowBalls(balls[i], balls[j], distance);
				ballsCollision(balls[i], balls[j]);
			}
		}
	}
}

NaiveBallCollissionStrategy::NaiveBallCollissionStrategy() : BallCollissionStrategy("naive")
{
}


NaiveBallCollissionStrategy::~NaiveBallCollissionStrategy() = default;
