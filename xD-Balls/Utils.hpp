#pragma once
#include <random>
#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"
#include "Ball.hpp"

#define P(x) std::cout<<#x<<": "<<x<<" "
#define D(x) std::cout<<#x<<": "<<x<<" "; std::cout.flush();

class Utils
{
public:
	static double dRand(const double fMin = 0, const double fMax = 1);
	static int rand(const int min = 0, const int max = 1);

	static std::string vector2DToString(const Vector2D& v);
	static std::string sfVector2fToString(const sf::Vector2f& v);
	static bool isVisable(const sf::View& view, const Vector2D& p);
	static bool isBallVisable(const sf::View& view, const Ball& b);

private:
	Utils();
	~Utils();

	static Utils& Instance();

	std::mt19937& get();

	Utils(Utils const&) = delete;
	Utils& operator= (Utils const&) = delete;

	std::mt19937 mt;
};
