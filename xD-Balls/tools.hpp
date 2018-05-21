#pragma once
#include "Ball.hpp"
#include "Environment.hpp"
#include "Utils.hpp"

enum ToolID {BALL_TOOL, RECTANGLE_TOOL, LINE_TOOL};

class Tool {

protected:
	Environment *e;
	Vector2D start_point;
	Vector2D end_point;
	sf::Vector2f worldPos;

	Vector2D getLocalVector(sf::Event::MouseButtonEvent &mbe, sf::RenderWindow &renderer) {
		worldPos = renderer.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y));
		return Vector2D(worldPos.x, worldPos.y);
	};
	Vector2D getLocalVector(sf::Event::MouseMoveEvent &mbe, sf::RenderWindow &renderer) {
		worldPos = renderer.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y));
		return Vector2D(worldPos.x, worldPos.y);
	};

public:
	Tool(Environment *e) : e(e) {}
	virtual void update(sf::Event &event, sf::RenderWindow &renderer) = 0;
	virtual void draw( sf::RenderWindow &renderer) = 0;
};

class BallTool : public Tool {

private:
	Vector2D additionalVelocity;
	std::vector<Ball> buffor;

	sf::Texture texture;

	Ball template_ball;
	int balls_per_deploy = 100;
	bool randomRadius = false;
	bool randomVelocity = true;
	bool randomColor = true;

	bool draw_flag = false;

public:
	void setBallsPerDeploy(int n) { balls_per_deploy = n; }
	void setBallsRadius(double d) { template_ball.r = d; }
	void setBallsMass(double d) { template_ball.m = d; }
	void setBallsBounceFactor(double d) { template_ball.bounce_factor = d; }
	void setBallsRandomVelocity(bool b) { randomVelocity = b; }
	void setBallsRandomRadius(bool b) { randomRadius = b; }
	void setBallsRandomColor(bool b) { randomColor = b; }


	BallTool(Environment *e) : Tool(e) {
		template_ball.collided = false;
		texture.loadFromFile("textures\\arrow.png");
	}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch(event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = true;
				start_point = getLocalVector(event.mouseButton, renderer);
				end_point = start_point;
				template_ball.position = start_point;
				buffor.reserve(balls_per_deploy);
				int temp = balls_per_deploy;//thats just to temporary fix issue with spawning lots of balls with enabled gravity forces
				if (e->settings.gravity_forces)
					temp = 1;
				for (int i = 0; i < temp; i++) {
					Ball ball(template_ball);
					if (randomVelocity)
						ball.velocity.setTryg(randFromTo(20, 200), randTo(M_PI * 2));
					if (randomColor)
						ball.color = sf::Color(randFromTo(20, 255), randFromTo(80, 255), randFromTo(80, 255));
					if (randomRadius)
						ball.r = randFromTo(3, 15);
					buffor.push_back(ball);
				}
			} 
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !e->BSpwn.balls.empty()) {
					int temp = balls_per_deploy;//thats just to temporary fix issue with spawning lots of balls with enabled gravity forces
					if (e->settings.gravity_forces)
						temp = 1;
					for (int i = 0; i < temp; i++)
						e->BSpwn.balls.pop_back();
				}
				else
					e->BSpwn.balls.clear();
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
				e->BSpwn.balls.reserve(buffor.size());
				for (Ball &b : buffor) {
					b.velocity += additionalVelocity;
					e->BSpwn.balls.push_back(b);
				}
				buffor.clear();
				additionalVelocity = Vector2D(0, 0);
			}

			break;
		default: 
			break;
		}
	};
	virtual void draw(sf::RenderWindow &renderer) {
		if (!draw_flag) return;
		sf::CircleShape circle;
		for (Ball &ball : buffor) {
			circle.setRadius(ball.r);
			circle.setOrigin(circle.getRadius(), circle.getRadius());
			circle.setPosition(ball.position.x, ball.position.y);
			circle.setFillColor(ball.color);
			renderer.draw(circle);
		}
		//drawing nice arrow
		sf::RectangleShape rect(sf::Vector2f((start_point-end_point).magnitude(), 50));
		rect.setPosition(0, 0);
		rect.setFillColor(sf::Color::White);
		rect.setTexture(&texture);
		rect.setOrigin(rect.getSize().x, 25);
		rect.setPosition(start_point.x, start_point.y);
		rect.setRotation(180 + atan2f((start_point - end_point).y, (start_point - end_point).x)*180/M_PI);
		renderer.draw(rect);

	}
};

class RectangleTool : public Tool {
private:
	Rectangle rectangle;
	bool randomColor = true;

	bool draw_flag = false;

public:

	RectangleTool(Environment *e) : Tool(e) {}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = true;
				start_point = getLocalVector(event.mouseButton, renderer);
				rectangle.set(start_point, start_point);
				if (randomColor)
					rectangle.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
			}
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					e->rectangles.pop_back();
				else
					e->rectangles.clear();
			}

			break;
		case sf::Event::MouseMoved:
			end_point = getLocalVector(event.mouseMove, renderer);
			rectangle.set(start_point, end_point);
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = false;
				end_point = getLocalVector(event.mouseButton, renderer);
				rectangle.set(start_point, end_point);
				e->rectangles.push_back(rectangle);

			}
			break;
		default:
			break;
		}
	};
	virtual void draw(sf::RenderWindow &renderer) {
		if (!draw_flag) return;

		sf::RectangleShape rect(sf::Vector2f(rectangle.rect.w, rectangle.rect.h));
		rect.setPosition(rectangle.rect.x, rectangle.rect.y);
		rect.setFillColor(rectangle.color);
		renderer.draw(rect);
	}
};

class LineTool : public Tool {

private:
	Line line;
	bool randomColor = true;

	bool draw_flag = false;

public:

	LineTool(Environment *e) : Tool(e) {}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = true;
				start_point = getLocalVector(event.mouseButton, renderer);
				line.set(start_point, start_point + Vector2D(1, 1));
				if (randomColor)
					line.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
			}
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					e->lines.pop_back();
				else
					e->lines.clear();
			}
			break;
		case sf::Event::MouseMoved:
			if (draw_flag) {
				end_point = getLocalVector(event.mouseMove, renderer);
				line.set(start_point, end_point);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = false;
				end_point = getLocalVector(event.mouseButton, renderer);
				line.set(start_point, end_point);
				e->lines.push_back(line);

			}
			break;
		default:
			break;
		}
	};
	virtual void draw(sf::RenderWindow &renderer) {
		if (!draw_flag) return;

		sf::RectangleShape rect(sf::Vector2f(line.length, line.width));
		rect.setPosition(line.p1.x, line.p1.y);
		rect.setRotation(line.angle*180/M_PI);
		rect.setFillColor(line.color);
		renderer.draw(rect);
	}
};