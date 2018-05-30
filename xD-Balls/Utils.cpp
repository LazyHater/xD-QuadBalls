#include "Utils.hpp"

#include <chrono>
#include <random>
#include <iostream>
#include <sstream>


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

std::string Utils::vector2DToString(const Vector2D& v) {
	std::stringstream ss;
	ss << "[" << v.x << ", " << v.y << "]";
	return ss.str();
}

std::string Utils::vector2DToString3Df(const Vector2D& v) {
	std::stringstream ss;
	ss << "[" << std::showpoint << v.x / 10.0f << ", 2.0, " << std::showpoint << v.y / 10.0f << "]";
	return ss.str();
}

std::string Utils::sfVector2fToString(const sf::Vector2f& v) {
	std::stringstream ss;
	ss << "[" << v.x << ", " << v.y << "]";
	return ss.str();
}


bool Utils::isVisable(const sf::View& view, const Vector2D& p) {
	double half_w = view.getSize().x / 2.0f;
	double half_h = view.getSize().y / 2.0f;
	double x0 = view.getCenter().x - half_w;
	double y0 = view.getCenter().y - half_h;
	double x1 = view.getCenter().x + half_w;
	double y1 = view.getCenter().y + half_h;
	if (p.x < x0 || p.x > x1) return false;
	if (p.y < y0 || p.y > y1) return false;
	return true;
}

bool Utils::isBallVisable(const sf::View& view, const Ball& b) {
	double half_w = view.getSize().x / 2.0f;
	double half_h = view.getSize().y / 2.0f;
	double x0 = view.getCenter().x - half_w;
	double y0 = view.getCenter().y - half_h;
	double x1 = view.getCenter().x + half_w;
	double y1 = view.getCenter().y + half_h;
	if (b.position.x + b.r < x0 || b.position.x - b.r > x1) return false;
	if (b.position.y + b.r < y0 || b.position.y - b.r > y1) return false;
	return true;
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
