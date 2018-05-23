#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "Ball.hpp"
#include "Environment.hpp"
#include "Utils.hpp"

class Tool
{
public:
	enum ToolType { BALL_TOOL, RECTANGLE_TOOL, LINE_TOOL, ATTRACTION_TOOL
	};

	const std::string name;

	Tool(Environment *e, const std::string name) : e(e), name(name) {}

	virtual void update(const sf::Event &event, const sf::RenderWindow &renderer) = 0;
	virtual void draw(sf::RenderWindow &renderer) = 0;

protected:
	Environment * e;
	Vector2D start_point;
	Vector2D end_point;
	sf::Vector2f worldPos;

	Vector2D getLocalVector(const sf::Event::MouseButtonEvent & mbe, const sf::RenderWindow & renderer);
	Vector2D getLocalVector(const sf::Event::MouseMoveEvent & mbe, const sf::RenderWindow & renderer);
};
