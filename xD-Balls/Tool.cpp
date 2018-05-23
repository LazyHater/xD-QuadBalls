#include "Tool.hpp"

Vector2D Tool::getLocalVector(const sf::Event::MouseButtonEvent & mbe, const sf::RenderWindow & renderer) {
	worldPos = renderer.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y));
	return Vector2D(worldPos.x, worldPos.y);
}

Vector2D Tool::getLocalVector(const sf::Event::MouseMoveEvent & mbe, const sf::RenderWindow & renderer) {
	worldPos = renderer.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y));
	return Vector2D(worldPos.x, worldPos.y);
}
