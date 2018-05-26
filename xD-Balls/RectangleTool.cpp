#include "RectangleTool.hpp"

RectangleTool::RectangleTool(World * w) : Tool(w) {}

RectangleTool::~RectangleTool() = default;

void RectangleTool::handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left) {
			draw_flag = true;
			start_point = getLocalVector(event.mouseButton, renderer);
			rectangle.set(start_point, start_point);
			if (randomColor)
				rectangle.color = sf::Color(Utils::rand(20, 255), 0, Utils::rand(80, 255));
		}
		else 	if (event.mouseButton.button == sf::Mouse::Right) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				w->rectangles.pop_back();
			else
				w->rectangles.clear();
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
			w->rectangles.push_back(rectangle);
		}
		break;
	default:
		break;
	}
}

void RectangleTool::draw(sf::RenderWindow & renderer) {
	if (!draw_flag) return;

	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rectangle.rect.w), static_cast<float>(rectangle.rect.h)));
	rect.setPosition(static_cast<float>(rectangle.rect.x), static_cast<float>(rectangle.rect.y));
	rect.setFillColor(rectangle.color);
	renderer.draw(rect);
}

std::string RectangleTool::getName() {
	return std::string("RectangleTool");
}

std::string RectangleTool::getStatusAsString() {
	return std::string();
}

void RectangleTool::update(double dt) {}
