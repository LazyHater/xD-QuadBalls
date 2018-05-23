#pragma once

#include <SFML/Graphics.hpp>

class Time
{
public:
	Time();
	~Time();

	void update();

	double getDeltaT() const { return delta_t.asSeconds(); }
	double getCurrentFps() const { return currentFPS; }
	double getAvgFps() const;

	void setTimeFactor(double val) { time_factor = val; }
	double getTimeFactor() const { return time_factor; }

private:
	static const size_t FPS_SAMPLES = 10;
	double samples[FPS_SAMPLES] = { 0.0f };
	size_t next_sample_idx = 0;

	sf::Time delta_t = sf::seconds(0.0f);
	sf::Time max_fps = sf::seconds(60.0f); // max on 60 fps
	sf::Time max_frame_time = sf::seconds(1.0f / 60.0f); // max on 60 fps
	double currentFPS;
	double time_factor = 1.f;
	sf::Clock clock;
	bool holdFPS = true;

	void newSample(const double sample);
};

