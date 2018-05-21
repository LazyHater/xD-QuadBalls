#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "qtree.hpp"

using namespace tml;
double frand(double a, double b) {
	return a + (double)rand() / RAND_MAX * (b - a);
}

class Obj {};

int main()
{
	/*
	std::vector<Obj> vec;
	for (int i = 0; i < 10; i++) {
		vec.push_back(Obj());
	}

	int idx = 0;
	for (auto& o : vec) {
		printf("%i: %p %p\n", idx, &o, &vec[idx]);
	}

	int a;
	cin >> a;
	return 0; */

	srand(time(NULL));
	Simulation sim(sf::VideoMode(1920, 1080), true);
	sim.process();

	return 0;
}