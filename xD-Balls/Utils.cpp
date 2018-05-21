#include <cmath>

float randTo(float to) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / to));
}

float randFromTo(float from, float to)
{
	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}