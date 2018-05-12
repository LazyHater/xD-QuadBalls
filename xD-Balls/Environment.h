#pragma once
#include "Line.h"
#include "BallSpawner.h"
#include "Rectangle.h"

struct Settings {
	int precision_of_calcs = 1;  //the more the better
	Vector2D gravity_vector = Vector2D(0,0);  //gravity like on earth 
	bool gravity_forces = false; // gravity like in solar system
	bool ball_to_ball_collisions = false;

	float G = 100;
	float force_max = 10000;	
};
class Environment {
	void handleCollisionWithScreen(std::vector<Ball> &balls);
	void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> lines);
	void handleCollisionWithRectangles(std::vector<Ball> &balls, std::vector <Rectangle> &rectangles);
	void handleGravityForces(std::vector<Ball> &balls);
public:
	BallSpawner BSpwn;
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;

	Settings settings;

	Rectangle box; //area where objects can move and exist

	Environment(Rectangle box);

	void create(Rectangle box);

	void update(float delta_t);
};


