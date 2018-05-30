#pragma once
#include "Tool.hpp"
#include "World.hpp"
#include "Ball.hpp"

class DebugTool :
	public Tool
{
public:
	DebugTool(World* w);
	virtual ~DebugTool();

	// Inherited via Tool
	virtual void update(double dt) override;
	virtual void handleEvent(const sf::Event & event, const sf::RenderWindow & renderer) override;
	virtual void draw(sf::RenderWindow & renderer) override;
	virtual std::string getName() override;
	virtual std::string getStatusAsString() override;
	void saveToFile(const std::string& fname);

private:
	void addPoint(Vector2D& p);
	std::vector<Ball> balls;
	const std::string fname = "L:/home/wakabajaszi/ClusteredRayTracer/Docker/src/cam.data";
	bool active = false;
	bool ppm = false;
};

