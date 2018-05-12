#pragma once
#pragma once
#include <Ball.h>
#include "Environment.h"
#include <QObject>

enum ToolID { BALL_TOOL, RECTANGLE_TOOL, LINE_TOOL };

class Tool : public QObject {

protected:
	Environment *e;
	Vector2D start_point;
	Vector2D end_point;
	sf::Vector2f worldPos;

public:
	Tool(Environment *e) : e(e) {}
	virtual void update(sf::Event &event, sf::RenderWindow &renderer) = 0; //returns true when smth has been changed in environment
	virtual void draw(sf::RenderWindow &renderer) = 0;
};

class BallTool : public Tool {
	Q_OBJECT

private:
	Vector2D additionalVelocity;
	std::vector<Ball> buffor;

	Ball template_ball;
	int balls_per_deploy;
	bool randomRadius = false;
	bool randomVelocity = true;
	bool randomColor = true;

	public slots:
	void setBallsPerDeploy(int n) { balls_per_deploy = n; }
	void setBallsRadius(double d) { template_ball.r = d; }
	void setBallsMass(double d) { template_ball.m = d; }
	void setBallsBounceFactor(double d) { template_ball.bounce_factor = d; }
	void setBallsRandomVelocity(bool b) { randomVelocity = b; }
	void setBallsRandomRadius(bool b) { randomRadius = b; }
	void setBallsRandomColor(bool b) { randomColor = b; }

signals:
	void ballsNChanged(int);

public:

	BallTool(Environment *e) : Tool(e) {
		//template_ball.r = 5;
		//template_ball.bounce_factor = 1.f;
		//template_ball.m = 100;
		//balls_per_deploy = 100;
		template_ball.collided = false;
	}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				start_point = Vector2D(worldPos.x, worldPos.y);

				template_ball.position = start_point;
				buffor.reserve(balls_per_deploy);
				for (int i = 0; i < balls_per_deploy; i++) {
					Ball ball(template_ball);
					if (randomVelocity)
						ball.velocity.setTryg(randFromTo(20, 200), randTo(M_PI * 2));
					if (randomColor)
						ball.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
					if (randomRadius)
						ball.r = randFromTo(3, 15);
					buffor.push_back(ball);
				}
			}
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					e->BSpwn.balls.pop_back();
				else
					e->BSpwn.balls.clear();
				emit ballsNChanged(e->BSpwn.balls.size());
			}

			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				end_point = Vector2D(worldPos.x, worldPos.y);
				additionalVelocity = start_point - end_point;
				e->BSpwn.balls.reserve(buffor.size());
				for (Ball &b : buffor) {
					b.velocity += additionalVelocity;
					e->BSpwn.balls.push_back(b);
				}
				emit ballsNChanged(e->BSpwn.balls.size());
				buffor.clear();
				additionalVelocity = Vector2D(0, 0);
			}

			break;
		default:
			break;
		}
	};
	virtual void draw(sf::RenderWindow &renderer) {
		sf::CircleShape circle;
		for (Ball &ball : buffor) {
			circle.setRadius(ball.r);
			circle.setOrigin(circle.getRadius(), circle.getRadius());
			circle.setPosition(ball.position.x, ball.position.y);
			circle.setFillColor(ball.color);
			renderer.draw(circle);
		}

	}
};

class RectangleTool : public Tool {
	Q_OBJECT

private:
	Rectangle rectangle;
	bool randomColor = true;

	bool draw_flag = false;
	public slots:

	signals:
		   void rectanglesNChanged(int);

public:

	RectangleTool(Environment *e) : Tool(e) {}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = true;
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				start_point = Vector2D(worldPos.x, worldPos.y);
				rectangle.set(start_point, start_point);
				if (randomColor)
					rectangle.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
			}
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					e->rectangles.pop_back();
				else
					e->rectangles.clear();
				emit rectanglesNChanged(e->rectangles.size());
			}

			break;
		case sf::Event::MouseMoved:
			worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			end_point = Vector2D(worldPos.x, worldPos.y);
			rectangle.set(start_point, end_point);
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = false;
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				end_point = Vector2D(worldPos.x, worldPos.y);
				rectangle.set(start_point, end_point);
				e->rectangles.push_back(rectangle);

				emit rectanglesNChanged(e->rectangles.size());
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
	Q_OBJECT

private:
	Line line;
	bool randomColor = true;

	bool draw_flag = false;
	public slots:

	signals:
		   void linesNChanged(int);

public:

	LineTool(Environment *e) : Tool(e) {}

	virtual void update(sf::Event &event, sf::RenderWindow &renderer) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = true;
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				start_point = Vector2D(worldPos.x, worldPos.y);
				line.set(start_point, start_point + Vector2D(1, 1));
				if (randomColor)
					line.color = sf::Color(randFromTo(20, 255), 0, randFromTo(80, 255));
			}
			else 	if (event.mouseButton.button == sf::Mouse::Right) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					e->lines.pop_back();
				else
					e->lines.clear();
				emit linesNChanged(e->lines.size());
			}
			break;
		case sf::Event::MouseMoved:
			if (draw_flag) {
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				end_point = Vector2D(worldPos.x, worldPos.y);
				line.set(start_point, end_point);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				draw_flag = false;
				worldPos = renderer.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				end_point = Vector2D(worldPos.x, worldPos.y);
				line.set(start_point, end_point);
				e->lines.push_back(line);

				emit linesNChanged(e->lines.size());
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
		rect.setRotation(line.angle * 180 / M_PI);
		rect.setFillColor(line.color);
		renderer.draw(rect);
	}
};