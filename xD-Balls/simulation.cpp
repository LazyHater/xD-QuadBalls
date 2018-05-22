#include "simulation.hpp"
#include <sstream>
const std::string HELP_MSG(
	R"(Navigation
Arrows - navigate
Scrool - up/down - zoom in/out
Scrool - hold - navigate
LPM - on press - choose direction and velocity for new balls
LPM - on release - deploy new balls
PPM - reset simulation

Funcionality
H - print this help
D - print debug info
P - pause simulation
R - reset simulation
1 - set collision strategy to disable
2 - set collision strategy to naive
3 - set collision strategy to qtree
4 - set collision strategy to parallel_qtree
F5 - quick save of simulation state
F7 - quick load of simulation state
ESC - quit simulation
)"
);

Simulation::Simulation(sf::VideoMode vm, bool full_screen) : 
	video_mode(vm), 
	full_screen(full_screen), 
	environment(Rectangle(0, 0, vm.width, vm.height)),
	ball_tool(&environment), 
	rectangle_tool(&environment), 
	line_tool(&environment) 
{
	environment.settings.ball_to_ball_collisions = true;

	ball_tool.setBallsMass(1);
	ball_tool.setBallsPerDeploy(100);
	ball_tool.setBallsRadius(1);
	ball_tool.setBallsBounceFactor(1);
	current_tool = &ball_tool;

	font.loadFromFile("fonts\\Roboto\\Roboto-Light.ttf");

	int textures_n = 12;
	this->ball_textures.resize(textures_n);
	std::cout << "vsize: " << this->ball_textures.size();
	for (int i = 0; i < textures_n; i++) {
		std::ostringstream ss;
		ss << "textures\\ball\\" << i << ".png";
		printf("loading asset: %s\n", ss.str().c_str());
		this->ball_textures[i].loadFromFile(ss.str());
		this->ball_textures[i].setSmooth(true);
	}

	ball_tool.setBallsTexturesNumber(textures_n);

	view.reset(sf::FloatRect(0, 0, vm.width, vm.height));
}

Simulation::~Simulation() {}

void Simulation::saveSimState(std::string s) {
	ofstream ofs(s, ios::binary);
	size_t n = environment.BSpwn.balls.size();
	ofs.write((char *)&n, sizeof(n));
	for (int i = 0; i < n; i++)
		ofs.write((char *)&environment.BSpwn.balls[i], sizeof(Ball));
	ofs.close();
}

void Simulation::loadSimState(std::string s) {
	ifstream ifs(s, ios::binary);
	size_t n;
	ifs.read((char *)&n, sizeof(n));

	environment.BSpwn.balls.clear();
	environment.BSpwn.balls.resize(n);
	for (int i = 0; i < n; i++)
		ifs.read((char *)&environment.BSpwn.balls[i], sizeof(Ball));
	ifs.close();
}

void Simulation::process() {
	sf::RenderWindow renderer;
	sf::VideoMode::getFullscreenModes()[0];
	if (full_screen) {
		renderer.create(video_mode, "Simulate Window", sf::Style::Fullscreen);
	}
	else {
		renderer.create(video_mode, "Simulate Window");
	}

	renderer.setView(view);
	while (renderer.isOpen())
	{
		sf::Event event;
		while (renderer.pollEvent(event)) //event loop
		{
			switch (event.type) {
			case sf::Event::Closed:
				renderer.close();
				break;

			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			case sf::Event::MouseMoved:
				static sf::Event::MouseMoveEvent prev_mouse_move;
				current_tool->update(event, renderer);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
					view.move((prev_mouse_move.x - event.mouseMove.x)*scale,
						(prev_mouse_move.y - event.mouseMove.y)*scale);
					renderer.setView(view);
				}
				prev_mouse_move = event.mouseMove;
				break;

			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta < 0) {
						view.zoom(2.0f);
						scale *= 2.f;
					}
					else {
						view.zoom(0.5f);
						scale *= 0.5f;
					}
					renderer.setView(view);
				}
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Up: // move camera up
					view.move(0, -20);
					renderer.setView(view);
					break;

				case sf::Keyboard::Down: // move camera down
					view.move(0, 20);
					renderer.setView(view);
					break;

				case sf::Keyboard::Left: // move camera left
					view.move(-20, 0);
					renderer.setView(view);
					break;

				case sf::Keyboard::Right: // move camera right
					view.move(20, 0);
					renderer.setView(view);
					break;

				case sf::Keyboard::Escape: // exit 
					quit = true;
					break;

				case sf::Keyboard::F5: // quick save
					saveSimState(quick_save_path);
					break;

				case sf::Keyboard::F7: // quick load
					loadSimState(quick_save_path);
					break;

				case sf::Keyboard::D: // enable / disable debug printing
					print_debug = !print_debug;
					print_help = false;
					break;

				case sf::Keyboard::H: // enable / disable help printing
					print_help = !print_help;
					print_debug = false;
					break;

				case sf::Keyboard::P: // pause
					paused = !paused;
					break;

				case sf::Keyboard::R: // pause
					this->environment.BSpwn.balls.clear();
					break;

				case sf::Keyboard::Num1: // set collision strategy to disable
					this->environment.setCurrentBallCollissionStrategy(Environment::CollisionStrategyType::Disabled);
					break;

				case sf::Keyboard::Num2: // set collision strategy to naive
					this->environment.setCurrentBallCollissionStrategy(Environment::CollisionStrategyType::Naive);
					break;

				case sf::Keyboard::Num3: // set collision strategy to qtree
					this->environment.setCurrentBallCollissionStrategy(Environment::CollisionStrategyType::Qtree);
					break;

				case sf::Keyboard::Num4: // set collision strategy to parallel_qtree
					this->environment.setCurrentBallCollissionStrategy(Environment::CollisionStrategyType::ParallelQtree);
					break;
				}
			}
			break;
		}

		environment.update(time.getDeltaT()*((this->paused) ? 0 : 1.f));//if paused then make 0 delta t

		renderer.clear();
		drawBalls(renderer, environment.BSpwn.balls);
		drawRectangles(renderer, environment.rectangles);
		drawLines(renderer, environment.lines);
		current_tool->draw(renderer);

		if (this->print_debug) { // print debug info
			std::ostringstream ss;
			ss << "FPS: " << time.getAvgFps() << "\n";
			ss << "Balls N: " << this->environment.BSpwn.balls.size() << "\n";
			ss << "Ball Strategy: " << this->environment.getCurrentBallCollissionStrategyName();
			this->drawText(renderer, 0, 0, ss.str());
		}

		if (this->print_help) { // print help message
			this->drawText(renderer, 0, 0, HELP_MSG);
		}
		renderer.display();

		time.update();

		if (quit) {
			renderer.close();
			quit = false;
		}
	}
}

void Simulation::setTool(int ID) {
	switch (ID) {
	case BALL_TOOL:
		current_tool = &ball_tool;
		break;
	case RECTANGLE_TOOL:
		current_tool = &rectangle_tool;
		break;
	case LINE_TOOL:
		current_tool = &line_tool;
		break;
	}
}


void Simulation::drawBalls(sf::RenderWindow &renderer, std::vector<Ball> &balls) {
	sf::CircleShape circle;
	for (Ball &ball : balls) {
		circle.setTexture(&this->ball_textures[ball.texture_id]);
		circle.setRadius(ball.r);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(ball.position.x, ball.position.y);
		circle.setFillColor(ball.color);
		renderer.draw(circle);
	}
}

void Simulation::drawRectangles(sf::RenderWindow &renderer, std::vector<Rectangle> &Rectangles) {
	sf::RectangleShape rect;
	for (Rectangle &rectangle : Rectangles) {
		rect.setSize(sf::Vector2f(rectangle.rect.w, rectangle.rect.h));
		rect.setPosition(rectangle.rect.x, rectangle.rect.y);
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineColor(rectangle.color);
		rect.setOutlineThickness(10);
		//rect.getOutlineColor
		renderer.draw(rect);
	}
}

void Simulation::drawLines(sf::RenderWindow &renderer, std::vector<Line> &Lines) {
	sf::RectangleShape rect;
	for (Line &line : Lines) {
		rect.setSize(sf::Vector2f(line.length, line.width));
		rect.setPosition(line.p1.x, line.p1.y);
		rect.setRotation(line.angle * 180 / M_PI);
		rect.setFillColor(line.color);
		renderer.draw(rect);
	}
}


void Simulation::drawText(sf::RenderWindow &renderer, int x, int y, const std::string s) {
	sf::View prev_state;
	prev_state = renderer.getView();
	renderer.setView(renderer.getDefaultView());

	sf::Text text;

	text.setFont(font);
	text.setString(s);
	text.setOutlineColor(sf::Color::Black);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(20);
	text.setPosition(x, y);
	renderer.draw(text);

	renderer.setView(prev_state);
}