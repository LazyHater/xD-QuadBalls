#pragma once
#include "Line.hpp"
#include "BallSpawner.hpp"
#include "Rectangle.hpp"
#include "NaiveBallCollissionStrategy.hpp"
#include "QtreeBallCollissionStrategy.hpp"

struct Settings {
	int precision_of_calcs = 1;  //the more the better precision
	Vector2D gravity_vector = Vector2D(0,0);  //gravity like on earth 
	bool gravity_forces = false; // gravity like in solar system
	bool ball_to_ball_collisions = false;

	float G = 100;
	float force_max = 10000;	
};

class Environment {
public:
	Environment(Rectangle box);

	BallSpawner BSpwn;
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;
	BallCollissionStrategy* current_ball_strategy;
	Settings settings;
	Rectangle bbox; //area where objects can move and exist


	void create(Rectangle box);
	void update(float delta_t);

private:
	NaiveBallCollissionStrategy naive_ball_strategy;
	QtreeBallCollissionStrategy qtree_ball_strategy;

	void handleCollisionWithScreen(std::vector<Ball> &balls);
	void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> lines);
	void handleCollisionWithRectangles(std::vector<Ball> &balls, std::vector <Rectangle> &rectangles);
	void handleGravityForces(std::vector<Ball> &balls);

};


