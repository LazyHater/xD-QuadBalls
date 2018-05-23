#include "Utils.hpp"

#include <chrono>
#include <random>
#include <iostream>

std::mt19937 & Utils::get() {
	return mt;
}

double Utils::dRand(const double min, const double max) {
	std::uniform_real_distribution<double> dis(min, max);
	return dis(Instance().get());
}

int Utils::rand(const int min, const int max) {
	std::uniform_int_distribution<int> dis(min, max);
	return dis(Instance().get());
}

Utils::Utils() {
	std::random_device rd;

	if (rd.entropy() != 0) {
		mt.seed(rd());
	}
	else {
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		mt.seed(static_cast<unsigned int>(seed));
	}
}

Utils::~Utils() {
}

Utils & Utils::Instance() {
	static Utils s;
	return s;
}
