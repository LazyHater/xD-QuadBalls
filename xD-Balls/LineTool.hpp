#pragma once
#include "Tool.hpp"

class LineTool : public Tool
{
public:
	LineTool(Environment *e) : Tool(e, "LineTool") {}
	virtual ~LineTool();

	virtual void update(const sf::Event & event, const sf::RenderWindow & renderer) override;
	virtual void draw(sf::RenderWindow & renderer) override;

private:
	Line line;
	bool randomColor = true;
	bool draw_flag = false;
};