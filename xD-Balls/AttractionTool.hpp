#pragma once
#include "Tool.hpp"

class AttractionTool :
	public Tool
{
public:
	explicit AttractionTool(World *e);
	virtual ~AttractionTool();

	virtual void update(double dt) override;
	virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& renderer) override;
	virtual void draw(sf::RenderWindow& renderer) override;

private:
	double force_strenght = 500.0f;
	bool active = false;
	bool ppm = true;
};

