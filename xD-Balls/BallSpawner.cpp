#include "BallSpawner.hpp"
#include "Qtree.hpp"
#include "Utils.hpp"


void BallSpawner::deployBalls(const Ball source, int n, bool random_velocity, bool random_radius, bool random_color, int texture_max_id) {
	balls.reserve(n);
	for (int i = 0; i < n; i++) {
		Ball ball(source);
		if (random_velocity)
			ball.velocity.setTryg(randFromTo(20, 200), randTo(M_PI * 2));
		if (random_color)
			ball.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
		if (random_radius)
			ball.r = randFromTo(3, 15);		

		ball.texture_id = randFromTo(0, (int)texture_max_id + 1);
		printf("%i\n", ball.texture_id);

		balls.push_back(ball);
	}
}

void BallSpawner::update(float delta_t) {
	for (Ball &ball : balls) {
		ball.update(delta_t);
	}
}

