#include <fstream>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>


#include "DebugTool.hpp"


DebugTool::DebugTool(World* w) : Tool(w) {}


DebugTool::~DebugTool() = default;

void DebugTool::update(double dt) {
}

void DebugTool::handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:	
		start_point = getLocalVector(event.mouseButton, renderer);
		if (event.mouseButton.button == sf::Mouse::Right) {
			balls.clear();
		}
		else if (event.mouseButton.button == sf::Mouse::XButton1) {
			this->saveToFile(this->fname);
			printf("Saved to file %s\n", this->fname.c_str());
		}
		else {
			active = true;
			this->addPoint(start_point);
		}
		break;

	case sf::Event::MouseButtonReleased:
		active = false;
		break;

	case sf::Event::MouseMoved:
		if (active) {
			start_point = getLocalVector(event.mouseMove, renderer);
			this->addPoint(start_point);
		}
		break;
	}
}

#define M_PI 3.14159265358979323846

void DebugTool::draw(sf::RenderWindow& renderer) { // ugly as f
	sf::CircleShape circle;

	Line l1(Vector2D(-100, 0), Vector2D(100, 0), sf::Color::Red);
	Line l2(Vector2D(0, -100), Vector2D(0, 100), sf::Color::Red);
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(static_cast<float>(l1.length), static_cast<float>(l1.width)));
	rect.setPosition(static_cast<float>(l1.p1.x), static_cast<float>(l1.p1.y));
	rect.setRotation(static_cast<float>(l1.angle * 180.0f / M_PI));
	rect.setFillColor(l1.color);
	renderer.draw(rect);
	rect.setSize(sf::Vector2f(static_cast<float>(l2.length), static_cast<float>(l2.width)));
	rect.setPosition(static_cast<float>(l2.p1.x), static_cast<float>(l2.p1.y));
	rect.setRotation(static_cast<float>(l2.angle * 180.0f / M_PI));
	rect.setFillColor(l2.color);
	renderer.draw(rect);


	for (const Ball& ball : balls) {
		float x = static_cast<float>(ball.position.x);
		float y = static_cast<float>(ball.position.y);
		float r = 10;
		circle.setRadius(r);
		circle.setPosition(x - r, y - r);
		circle.setFillColor(sf::Color::White);
		renderer.draw(circle);
	}
}

std::string DebugTool::getName() {
	return std::string("DebugTool");
}

std::string DebugTool::getStatusAsString() {
	std::ostringstream ss;
	ss << "Samples: " << balls.size() << "\n";
	if (balls.size() > 0) {
		ss << Utils::vector2DToString(balls[0].position);
	}
	return ss.str();
}

void DebugTool::saveToFile(const std::string& fname) {
	std::ofstream myfile;
	myfile.open(fname);

	myfile << "[\n";
	for (int i = 0; i + 1 < balls.size(); i++) {
		myfile << Utils::vector2DToString3Df(balls[i].position) << ",\n";
	}
	myfile << Utils::vector2DToString3Df(balls.back().position) << "\n]\n";
	myfile.close();
}

void DebugTool::addPoint(Vector2D& p) {
	Ball b;
	b.position = p;
	balls.push_back(b);
}
