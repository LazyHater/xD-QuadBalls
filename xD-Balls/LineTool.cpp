#include "LineTool.hpp"

LineTool::LineTool(World * w) : Tool(w) {}

LineTool::~LineTool() = default;

void LineTool::handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left) {
			draw_flag = true;
			start_point = getLocalVector(event.mouseButton, renderer);
			line.set(start_point, start_point + Vector2D(1, 1));
			if (randomColor)
				line.color = sf::Color(Utils::rand(20, 255), 0, Utils::rand(80, 255));
		}
		else 	if (event.mouseButton.button == sf::Mouse::Right) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				w->lines.pop_back();
			else
				w->lines.clear();
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
			w->lines.push_back(line);

		}
		break;
	default:
		break;
	}
}

void LineTool::draw(sf::RenderWindow & renderer) {
	if (!draw_flag) return;

	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(line.length), static_cast<float>(line.width)));
	rect.setPosition(static_cast<float>(line.p1.x), static_cast<float>(line.p1.y));
	rect.setRotation(static_cast<float>(line.angle * 180.0f / M_PI));
	rect.setFillColor(line.color);
	renderer.draw(rect);
}

std::string LineTool::getName() {
	return std::string("LineTool");
}

std::string LineTool::getStatusAsString() {
	return std::string();
}

void LineTool::update(double dt) {}
