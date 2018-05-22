#pragma once
#include <algorithm>
#include <iterator>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"
#include "Environment.hpp"
#include "tools.hpp"

using namespace std;


class Time {
public:
	Time() {
		max_frame_time = sf::seconds(1 / max_fps.asSeconds());
	}
	void update() {
		delta_t = clock.getElapsedTime();
		if (delta_t<max_frame_time)
			sf::sleep(max_frame_time - delta_t);
		delta_t = clock.restart();
		currentFPS = 1 / delta_t.asSeconds();
		newSample(currentFPS);
		delta_t *= time_factor;
	}



	float getDeltaT() { return delta_t.asSeconds(); }
	float getCurrentFps() { return currentFPS; }
	float getAvgFps() { 
		float sum = 0.0f;
		for (size_t i = 0; i < FPS_SAMPLES; i++) {
			sum += samples[i];
		}
		return sum / (float)FPS_SAMPLES;
	}

	void setTimeFactor(double val) { time_factor = val; }

private:
	static const size_t FPS_SAMPLES = 10;
	float samples[FPS_SAMPLES] = {0.0f};
	size_t next_sample_idx = 0;

	sf::Time delta_t = sf::seconds(0);
	sf::Time max_fps = sf::seconds(60.f); // max on 60 fps
	sf::Time max_frame_time = sf::seconds(1.f / 60.f); // max on 60 fps
	float currentFPS;
	float time_factor = 1.f;
	sf::Clock clock;
	bool holdFPS = true;

	void newSample(float sample) {
		samples[next_sample_idx] = sample;
		next_sample_idx++;
		next_sample_idx %= FPS_SAMPLES;
	}
};

class Simulation{

public:
	Simulation(sf::VideoMode vm, bool full_screen);
	~Simulation();


	std::string quick_save_path = "saves\\quick_save.sim";

	bool quit = false;

	void saveSimState(std::string s);
	void loadSimState(std::string s);

	void process();
	void setPause(bool b) { paused = b; }
	void setTool(int ID);

	void setTimeFactor(double val) { time.setTimeFactor(val); }
	void setVerySlowTimeFactor() { time.setTimeFactor(0.01); }
	void setSlowTimeFactor() { time.setTimeFactor(0.1);	}
	void setNormalTimeFactor() { time.setTimeFactor(1); }
	void setFastTimeFactor() { time.setTimeFactor(10); }

	void setPrintDebug(bool b) { print_debug = b; }
	void setBallToBallCollisions(bool b) { environment.settings.ball_to_ball_collisions = b; }
	void setGravityForces(bool b) {	environment.settings.gravity_forces = b; }

private:
	BallTool ball_tool;
	RectangleTool rectangle_tool;
	LineTool line_tool;

	bool paused = false;

	std::vector<sf::Texture> ball_textures;
	sf::View view;
	sf::VideoMode video_mode;
	bool full_screen;

	sf::Font font;
	bool print_debug = true;
	bool print_help = false;
	Time time;
	Environment environment;
	Tool *current_tool;

	float scale = 1.f;

	sf::Vector2i def_window_position;
	void drawBalls(sf::RenderWindow &renderer, std::vector<Ball> &balls);
	void drawRectangles(sf::RenderWindow &renderer, std::vector<Rectangle> &Rectangles);
	void drawLines(sf::RenderWindow &renderer, std::vector<Line> &Lines);

	void drawText(sf::RenderWindow &renderer, int x, int y, std::string s);
	// add your variables here
};


