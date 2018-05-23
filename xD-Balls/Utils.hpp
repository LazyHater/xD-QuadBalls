#pragma once
#include <random>

#define P(x) std::cout<<#x<<": "<<x<<" "
#define D(x) std::cout<<#x<<": "<<x<<" "; std::cout.flush();

class Utils
{
public:
	static double dRand(const double fMin = 0, const double fMax = 1);
	static int rand(const int min = 0, const int max = 1);

private:
	Utils();
	~Utils();

	static Utils& Instance();

	std::mt19937& get();

	Utils(Utils const&) = delete;
	Utils& operator= (Utils const&) = delete;

	std::mt19937 mt;
};
