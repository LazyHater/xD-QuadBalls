#define _USE_MATH_DEFINES
#include <cmath>
#include "BallSpawner.hpp"
#include "Qtree.hpp"
#include "Utils.hpp"


BallSpawner::BallSpawner() = default;

BallSpawner::~BallSpawner() = default;

void BallSpawner::deployBalls(const Ball source, int n, bool random_velocity, bool random_radius, bool random_color, size_t texture_max_id) {
	balls.reserve(n);
	for (int i = 0; i < n; i++) {
		Ball ball(source);
		if (random_velocity) {
			ball.velocity.setTryg(Utils::dRand(20, 200), Utils::dRand(0, M_PI * 2));
		}
		if (random_color) {
			ball.color = sf::Color(Utils::rand(20, 255), 0, Utils::rand(80, 255));
		}
		if (random_radius) {
			ball.r = Utils::dRand(3, 15);
		}

		ball.texture_id = Utils::rand(0, static_cast<const int>(texture_max_id + 1));
		printf("%i\n", ball.texture_id);

		balls.push_back(ball);
	}
}

void BallSpawner::update(double delta_t) {
	for (Ball &ball : balls) {
		ball.update(delta_t);
	}
}

