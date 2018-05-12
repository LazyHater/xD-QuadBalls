#include "Environment.h"

Environment::Environment(Rectangle box) : box(box) {
	this->box.color = sf::Color(255, 0, 0);
	//this->rectangles.push_back(this->box);
}

void Environment::create(Rectangle box) {
	this->box.color = sf::Color(255, 0, 0);
	BSpwn.balls.clear();
	lines.clear();
	rectangles.clear();
}

void Environment::handleCollisionWithScreen(std::vector<Ball>& balls) {
	for (Ball &ball : balls) {
		if (ball.position.x + ball.r > box.rect.x + box.rect.w) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = box.rect.x + box.rect.w - ball.r;
			ball.collided = true;
		}
		if (ball.position.x - ball.r < box.rect.x) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = box.rect.x + ball.r;
			ball.collided = true;
		}
		if (ball.position.y + ball.r > box.rect.y + box.rect.h) {
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = box.rect.y + box.rect.h - ball.r;
			ball.collided = true;
		}
		if (ball.position.y - ball.r < box.rect.y) {
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = box.rect.y + ball.r;
			ball.collided = true;
		}
	}
}

void Environment::handleCollisionWithLines(std::vector<Ball>& balls, std::vector<Line> lines) {
	for (Line line : lines) {
		for (Ball &ball : balls) {
			if (line.collideBox.isIn(ball.position)) {
				float distance = line.distance(ball.position) - line.width;
				if (distance < ball.r) {
					ball.velocity = ball.velocity - line.norVec*
						2.0*(ball.velocity*line.norVec); //R=V-2N(N*V)
					ball.collided = true;
					if (ball.velocity.angle(line.norVec) < M_PI / 2)
						ball.position += line.norVec*(ball.r - distance);
					else
						ball.position -= line.norVec*(ball.r - distance);
				}
			}
		}
	}
}

void Environment::handleCollisionWithRectangles(std::vector<Ball>& balls, std::vector<Rectangle>& rectangles) {
	for (Rectangle &rectangle : rectangles) {
		for (Ball &ball : balls) {
			if ((abs(rectangle.position.x - ball.position.x)<ball.r + rectangle.rect.w / 2.0f)
				&& (abs(rectangle.position.y - ball.position.y)<ball.r + rectangle.rect.h / 2.0f)) {

				if (abs(ball.position.x - rectangle.position.x + rectangle.rect.w / 2.0f) < ball.r) {
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;
					ball.position.x = rectangle.position.x - rectangle.rect.w / 2.0f - ball.r;
				}

				if (abs(ball.position.x - rectangle.position.x - rectangle.rect.w / 2.0f) < ball.r) {
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;
					ball.position.x = rectangle.position.x + rectangle.rect.w / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y - rectangle.rect.h / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;
					ball.position.y = rectangle.position.y + rectangle.rect.h / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y + rectangle.rect.h / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;
					ball.position.y = rectangle.position.y - rectangle.rect.h / 2.0f - ball.r;
				}
			}
		}
	}
}
void Environment::handleGravityForces(std::vector<Ball>& balls) {

	for (Ball &ball : balls)  ball.acceleration = Vector2D(0, 0);

	int temp = balls.size();
	for (int i = 0; i < temp - 1; i++) {
		for (int j = i + 1; j < temp; j++) {
			float r = Vector2D::distance(balls[i].position, balls[j].position);
			if (r <= balls[i].r + balls[j].r) r = balls[i].r + balls[j].r;

			float force = settings.G*(balls[i].m*balls[j].m) / (r*r); //G*(m1*m2)/r^2
			//if (force > settings.force_max) force = settings.force_max;

			Vector2D dir_vec = balls[j].position - balls[i].position; // vector from first ball to second
			Vector2D force_vec = dir_vec;
			force_vec.normalize();
			force_vec *= force;

			balls[i].acceleration += force_vec / balls[i].m;
			balls[j].acceleration += !force_vec / balls[j].m;
		}
	}

}

void Environment::update(float delta_t) {
	//float max_time = 1.f / 60.f;
	//if (delta_t > max_time) delta_t = max_time;	
	
		for (int i = 0; i < settings.precision_of_calcs; i++) {
			if (settings.gravity_forces)
				handleGravityForces(BSpwn.balls);
			if (settings.ball_to_ball_collisions)
				BSpwn.handleCollisionBallToBall();
			handleCollisionWithScreen(BSpwn.balls);
			handleCollisionWithLines(BSpwn.balls, lines);
			handleCollisionWithRectangles(BSpwn.balls, rectangles);
			BSpwn.update(delta_t / settings.precision_of_calcs);
			for (Ball &ball : BSpwn.balls) {
				ball.velocity += settings.gravity_vector*delta_t / settings.precision_of_calcs;
			}
		}
	

}
