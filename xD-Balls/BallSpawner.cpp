#include "BallSpawner.hpp"
#include "Qtree.hpp"
#include "Utils.hpp"

void BallSpawner::plowBalls(Ball &ball_1, Ball &ball_2, float distance) {
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

void BallSpawner::handleCollisionBallToBall() {
	int temp = balls.size();
	for (int i = 0; i < temp - 1; i++) {
		for (int j = i + 1; j < temp; j++) {
			float distance = balls[i].position.distance(balls[j].position);
			if (distance < balls[i].r + balls[j].r) {
				plowBalls(balls[i], balls[j], distance);
				ballsCollision(balls[i], balls[j]);
			}
		}
	}
}

void BallSpawner::handleCollisionBallToBallQuadTree(const Rectangle& box) {
	tml::qtree<double, Ball*> tree(box.rect.x - 100, box.rect.y - 100, box.rect.x + box.rect.w + 100, box.rect.y + box.rect.h + 100);
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

		for (auto  node  : result) {
			Ball* b2 = node->data;

			float distance = b1->position.distance(b2->position);
			if (distance < b1->r + b2->r) {
				plowBalls(*b1, *b2, distance);
				ballsCollision(*b1, *b2);
			}
		}


	}


	//printf("%d failed result size %d\n", fail_cnt, result.size());
	if (result.size() > 0)
	printf("result count: %f\n", result[0]->data->r);
}

void BallSpawner::ballsCollision(Ball &ball_1, Ball &ball_2) {
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

	if (ball_to_ball_bounce) {
		ball_1.collided = true;
		ball_2.collided = true;
	}
}

void BallSpawner::deployBalls(const Ball source, int n, bool random_velocity, bool random_radius, bool random_color) {
	balls.reserve(n);
	for (int i = 0; i < n; i++) {
		Ball ball(source);
		if (random_velocity)
			ball.velocity.setTryg(randFromTo(20, 200), randTo(M_PI * 2));
		if (random_color)
			ball.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
		if (random_radius)
			ball.r = randFromTo(3, 15);
		balls.push_back(ball);
	}
}

void BallSpawner::update(float delta_t) {
	for (Ball &ball : balls) {
		ball.update(delta_t);
	}
}

