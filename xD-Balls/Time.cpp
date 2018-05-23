#include "Time.hpp"

Time::Time() : max_frame_time(sf::seconds(1 / max_fps.asSeconds())) {}

Time::~Time() = default;

void Time::update() {
	delta_t = clock.getElapsedTime();
	if (delta_t < max_frame_time)
		sf::sleep(max_frame_time - delta_t);
	delta_t = clock.restart();
	currentFPS = 1 / delta_t.asSeconds();
	newSample(currentFPS);
	delta_t *= static_cast<float>(time_factor);
}

double Time::getAvgFps() const {
	double sum = 0.0f;
	for (size_t i = 0; i < FPS_SAMPLES; i++) {
		sum += samples[i];
	}
	return sum / (double)FPS_SAMPLES;
}

void Time::newSample(const double sample) {
	samples[next_sample_idx] = sample;
	next_sample_idx++;
	next_sample_idx %= FPS_SAMPLES;
}
