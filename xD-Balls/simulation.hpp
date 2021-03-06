﻿#pragma once
#include <algorithm>
#include <iterator>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "World.hpp"
#include "AverageFilter.hpp"
#include "FpsHandler.hpp"

#include "Tool.hpp"
#include "BallTool.hpp"
#include "LineTool.hpp"
#include "RectangleTool.hpp"
#include "AttractionTool.hpp"
#include "DebugTool.hpp"

using namespace std;

class Simulation
{
public:
	explicit Simulation(const sf::VideoMode vm, const bool full_screen);
	~Simulation(); 

	void saveSimState(const std::string s) const;
	void loadSimState(const std::string s);

	void mainLoop();

	inline void setPause(const bool b) { paused = b; }
	inline void setTool(const int id);
	inline void setTimeFactor(const double val) { time.setTimeFactor(val); }
	inline void setVerySlowTimeFactor() { time.setTimeFactor(0.01f); }
	inline void setSlowTimeFactor() { time.setTimeFactor(0.1f);	}
	inline void setNormalTimeFactor() { time.setTimeFactor(1.0f); }
	inline void setFastTimeFactor() { time.setTimeFactor(10.0f); }
	inline void setPrintDebug(const bool b) { print_debug = b; }
	inline void setGravityForces(const bool b) { world.settings.gravity_forces = b; }

private:
	BallTool ball_tool;
	LineTool line_tool;
	RectangleTool rectangle_tool;
	AttractionTool attraction_tool;
	DebugTool debug_tool;

	bool paused = false;
	bool quit = false;
	bool full_screen;

	const std::string quick_save_path = "saves\\quick_save.sim";

	std::vector<sf::Texture> ball_textures;
	sf::View view;
	const sf::VideoMode video_mode;

	sf::Font font;
	bool print_debug = true;
	bool print_help = false;
	bool draw_qtree = false;
	FpsHandler time;
	World world;
	Tool *current_tool;

	double scale = 1.0f;

	AverageFilter<sf::Int32> eventLoop_ms_time;
	AverageFilter<sf::Int32> update_ms_time;
	AverageFilter<sf::Int32> render_ms_time;

	void loadAssets();
	void eventLoop(sf::RenderWindow &renderer);
	void update();
	void render(sf::RenderWindow &renderer);

	void renderBalls(sf::RenderWindow &renderer, const std::vector<Ball>& balls);
	void renderRectangles(sf::RenderWindow &renderer, const std::vector<Rectangle> &Rectangles);
	void renderLines(sf::RenderWindow &renderer, const std::vector<Line> &Lines);

	void renderQtree(sf::RenderWindow &renderer, const std::vector<Ball>& balls);
	void renderText(sf::RenderWindow &renderer, const double x, const double y, const std::string s);
};


