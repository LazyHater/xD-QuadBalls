#include "AttractionTool.hpp"



AttractionTool::AttractionTool(World* w) : Tool(w) {}


AttractionTool::~AttractionTool() = default;

void AttractionTool::handleEvent(const sf::Event& event, const sf::RenderWindow& renderer) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		start_point = getLocalVector(event.mouseButton, renderer);
		active = true;
		ppm = (event.mouseButton.button == sf::Mouse::Right);
		break;

	case sf::Event::MouseButtonReleased:
		active = false;

		for (Ball &ball : this->w->BSpwn.balls) {
			ball.acceleration.set(0.0f, 0.0f);
		}
		break;

	case sf::Event::MouseMoved:
		start_point = getLocalVector(event.mouseMove, renderer);
		break;
	}
}

void AttractionTool::draw(sf::RenderWindow & renderer) {
	if (!active) return;

	sf::CircleShape circle;
	circle.setRadius(10.0f);
	circle.setPosition(static_cast<float>(start_point.x - 10.0f), static_cast<float>(start_point.y) - 10.0f);
	circle.setFillColor(sf::Color::Red);
	renderer.draw(circle);
}

std::string AttractionTool::getName() {
	return std::string("AttractionTool");
}

std::string AttractionTool::getStatusAsString() {
	return std::string();
}

void AttractionTool::update(double dt) {
	if (!active) return;

	for (Ball &ball : this->w->BSpwn.balls) {
		Vector2D direction = (start_point - ball.position).normalize();
		if (ppm) {
			direction.negate();
		}
		ball.acceleration = direction * force_strenght;
	}
}
