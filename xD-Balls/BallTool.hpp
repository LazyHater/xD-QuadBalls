#pragma once
#include "Tool.hpp"

class BallTool : public Tool
{
public:
	void setBallsPerDeploy(const int n) { balls_per_deploy = n; }
	void setBallsRadius(const double d) { template_ball.r = d; }
	void setBallsMass(const double d) { template_ball.m = d; }
	void setBallsBounceFactor(const double d) { template_ball.bounce_factor = d; }
	void setBallsRandomVelocity(const bool b) { randomVelocity = b; }
	void setBallsRandomRadius(const bool b) { randomRadius = b; }
	void setBallsRandomColor(const bool b) { randomColor = b; }
	void setBallsTexturesNumber(const int n) { textures_n = n; }

	explicit BallTool(World* e);
	~BallTool();

	virtual void update(double dt) override;
	virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& renderer) override;
	virtual void draw(sf::RenderWindow& renderer) override;

private:
	Vector2D additionalVelocity;
	std::vector<Ball> buffor;

	sf::Texture texture;
	Ball template_ball;
	size_t textures_n = 2;
	size_t balls_per_deploy = 100;
	bool randomRadius = false;
	bool randomVelocity = true;
	bool randomColor = true;
	bool draw_flag = false;
};