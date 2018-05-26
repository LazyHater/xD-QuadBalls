#pragma once
#include "Tool.hpp"

class RectangleTool : public Tool 
{
public:
	explicit RectangleTool(World *w);
	virtual ~RectangleTool();

	virtual void update(double dt) override;
	virtual void handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) override;
	virtual void draw(sf::RenderWindow &renderer) override;

	virtual std::string getName() override;
	virtual std::string getStatusAsString() override;

private:
	Rectangle rectangle;
	bool randomColor = true;
	bool draw_flag = false;

};