#include "AttractionTool.hpp"



AttractionTool::AttractionTool(Environment* e) : Tool(e, "AttractionTool") {
}


AttractionTool::~AttractionTool() = default;

void AttractionTool::update(const sf::Event& event, const sf::RenderWindow& renderer) {
	switch (event.type) {

	case sf::Event::MouseButtonPressed:
		active = true;
		break;

	case sf::Event::MouseButtonReleased:
		active = false;
		break;

	case sf::Event::MouseMoved:
		if (!active) {
			return;
		}
		start_point = getLocalVector(event.mouseButton, renderer);

		for (Ball &ball : this->e->BSpwn.balls) {
			Vector2D direction = (start_point - ball.position).normalize();
			ball.acceleration = direction * force_strenght;
		}
		break;
	}
}

void AttractionTool::draw(sf::RenderWindow & renderer) {
}
