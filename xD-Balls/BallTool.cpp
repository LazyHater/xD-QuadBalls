#include "BallTool.hpp"

BallTool::BallTool(World* w) : Tool(w) {
	template_ball.collided = false;
	texture.loadFromFile("textures\\arrow.png");
}

BallTool::~BallTool() = default;

void BallTool::handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left) {
			draw_flag = true;
			start_point = getLocalVector(event.mouseButton, renderer);
			end_point = start_point;
			template_ball.position = start_point;
			buffor.reserve(balls_per_deploy);

			int temp = (w->settings.gravity_forces) ? 1 : balls_per_deploy;//thats just to temporary fix issue with spawning lots of balls with enabled gravity forces

			for (int i = 0; i < temp; i++) {
				Ball ball(template_ball);
				if (randomVelocity)
					ball.velocity.setTryg(Utils::dRand(20, 200), Utils::dRand(0, M_PI * 2));
				if (randomColor)
					ball.color = sf::Color(Utils::rand(20, 255), Utils::rand(80, 255), Utils::rand(80, 255));
				if (randomRadius)
					ball.r = Utils::dRand(3, 15);
				ball.texture_id = static_cast<unsigned int>(Utils::rand(0, textures_n - 1));

				buffor.push_back(ball);
			}
		}
		else 	if (event.mouseButton.button == sf::Mouse::Right) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !w->BSpwn.balls.empty()) {
				int temp = balls_per_deploy;//thats just to temporary fix issue with spawning lots of balls with enabled gravity forces
				if (w->settings.gravity_forces)
					temp = 1;
				for (int i = 0; i < temp; i++)
					w->BSpwn.balls.pop_back();
			}
			else {
				w->BSpwn.balls.clear();
			}
		}
		break;

	case sf::Event::MouseMoved:
		if (draw_flag) {
			end_point = getLocalVector(event.mouseMove, renderer);
		}
		break;

	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			draw_flag = false;
			end_point = getLocalVector(event.mouseButton, renderer);
			additionalVelocity = start_point - end_point;
			w->BSpwn.balls.reserve(buffor.size());
			for (Ball &b : buffor) {
				b.velocity += additionalVelocity;
				w->BSpwn.balls.push_back(b);
			}
			buffor.clear();
			additionalVelocity = Vector2D(0, 0);
		}

		break;
	default:
		break;
	}
}

void BallTool::draw(sf::RenderWindow & renderer) {
	if (!draw_flag) return;
	sf::CircleShape circle;
	for (const Ball &ball : buffor) {
		circle.setRadius(static_cast<float>(ball.r));
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(static_cast<float>(ball.position.x), static_cast<float>(ball.position.y));
		circle.setFillColor(ball.color);
		renderer.draw(circle);
	}
	//drawing nice arrow
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>((start_point - end_point).magnitude()), 50.0f));
	rect.setPosition(0.0f, 0.0f);
	rect.setFillColor(sf::Color::White);
	rect.setTexture(&texture);
	rect.setOrigin(rect.getSize().x, 25.0f);
	rect.setPosition(static_cast<float>(start_point.x), static_cast<float>(start_point.y));
	rect.setRotation(180.0f + atan2f(static_cast<float>((start_point - end_point).y), static_cast<float>((start_point - end_point).x)) * 180.0f / static_cast<float>(M_PI));
	renderer.draw(rect);
}

std::string BallTool::getName() {
	return std::string("BallTool");
}

std::string BallTool::getStatusAsString() {
	return std::string("xDDD");
}

void BallTool::update(double dt) {}
