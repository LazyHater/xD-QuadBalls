#pragma once
#include "Tool.hpp"

class AttractionTool :
	public Tool
{
public:
	explicit AttractionTool(Environment *e);
	virtual ~AttractionTool();

	virtual void update(const sf::Event& event, const sf::RenderWindow& renderer) override;
	virtual void draw(sf::RenderWindow& renderer) override;

private:
	double force_strenght = 1000.0f;
	bool active = false;
};

