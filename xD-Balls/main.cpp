#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "qtree.hpp"
#include "Utils.hpp"

int main() {

	Simulation sim(sf::VideoMode(1920, 1080), true);
	sim.mainLoop();

	return 0;
}