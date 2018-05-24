#pragma once
#include "Line.hpp"
#include "BallSpawner.hpp"
#include "Rectangle.hpp"
#include "DisabledBallCollissionStrategy.hpp"
#include "NaiveBallCollissionStrategy.hpp"
#include "QtreeBallCollissionStrategy.hpp"
#include "ParallelQtreeBallCollissionStrategy.hpp"

struct Settings
{
	unsigned int precision_of_calcs = 1;  //the more the better precision
	Vector2D gravity_vector = Vector2D(0,0);  //gravity like on earth 
	bool gravity_forces = false; // gravity like in solar system

	double G = 100;
	double force_max = 10000;	
};

class World
{
public:
	explicit World(Rectangle bbox);

	enum CollisionStrategyType {
		Disabled, Naive, Qtree, ParallelQtree
	};

	BallSpawner BSpwn;
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;
	Settings settings;
	Rectangle bbox; //area where objects can move and exist

	void update(const double delta_t);
	void setCurrentBallCollissionStrategy(CollisionStrategyType type);
    const std::string getCurrentBallCollissionStrategyName() const { return current_ball_strategy->name; }

private:
	BallCollissionStrategy* current_ball_strategy;
	DisabledBallCollissionStrategy disabled_ball_strategy;
	NaiveBallCollissionStrategy naive_ball_strategy;
	QtreeBallCollissionStrategy qtree_ball_strategy;
	ParallelQtreeBallCollissionStrategy parallel_qtree_ball_strategy;
	
	void handleCollisionWithScreen(std::vector<Ball> &balls);
	void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> lines);
	void handleCollisionWithRectangles(std::vector<Ball> &balls, std::vector <Rectangle> &rectangles);
	void handleGravityForces(std::vector<Ball> &balls);
};


