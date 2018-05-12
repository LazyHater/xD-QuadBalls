#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "qtree.h"

using namespace tml;
double frand(double a, double b) {
	return a + (double)rand() / RAND_MAX * (b - a);
}

int main()
{

	qtree<double, int> tree(0, 0, 10000, 10000);
	srand(time(NULL));
	/*
	for (int i = 0; i < 100000; i++) {
		double x = frand(0, 10000);
		double y = frand(0, 10000);
		int data = i;
		if (!tree.add_node(x, y, data)) {
			fail_cnt += 1;
		}
	}
	*/

	int fail_cnt = 0;
	if (!tree.add_node(100, 100, 1)) {
		fail_cnt += 1;
	}

	if (!tree.add_node(105, 100, 2)) {
		fail_cnt += 1;
	}

	if (!tree.add_node(110, 100, 3)) {
		fail_cnt += 1;
	}
	printf("%d failed\n", fail_cnt);

	std::vector<const qtree<double, int>::node_type *> result;
	tree.search(100, 100, 11, result);

	printf("result count: %d\n", result.size());

	for (int j = 0; j < result.size(); j++) {
		printf("x=%f, y=%f, v=%d, d=%f\n", result[j]->x, result[j]->y, result[j]->data, 10.0f);
	}

	int a;
	std::cin >> a;
	return 0;
	Simulation sim(sf::VideoMode(1920, 1080), true);
	sim.process();

	return 0;
}