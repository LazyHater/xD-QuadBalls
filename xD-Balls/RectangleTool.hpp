#pragma once
#include "Tool.hpp"

class RectangleTool : public Tool 
{
public:
	explicit RectangleTool(Environment *e) : Tool(e, "RectangleTool") {}
	virtual ~RectangleTool();

	virtual void update(const sf::Event & event, const sf::RenderWindow & renderer) override;
	virtual void draw(sf::RenderWindow &renderer) override;

private:
	Rectangle rectangle;
	bool randomColor = true;

	bool draw_flag = false;
};