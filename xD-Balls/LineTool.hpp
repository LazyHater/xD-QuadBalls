#pragma once
#include "Tool.hpp"

class LineTool : public Tool
{
public:
	LineTool(World *w) : Tool(w, "LineTool") {}
	virtual ~LineTool();

	virtual void update(double dt) override;
	virtual void handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) override;
	virtual void draw(sf::RenderWindow & renderer) override;

private:
	Line line;
	bool randomColor = true;
	bool draw_flag = false;
};