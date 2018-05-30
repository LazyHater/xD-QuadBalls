#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <thread>

#include "simulation.hpp"
#include "Qtree.hpp"

const std::string HELP_MSG(
	R"(Navigation
Arrows - navigate
Scrool - hold - navigate
Scrool - up/down - zoom in/out
LPM - tool specific
PPM - tool specific

Utility
H - print this help
D - print debug info
T - render qtree
F5 - quick save of simulation state
F7 - quick load of simulation state
ESC - quit simulation

Simulation
P/space - pause simulation
R - reset simulation
G - enable/disable gravity forces
+ - increase simulation speed
- - decrease simulation speed

Collisions
1 - set collision strategy to disable
2 - set collision strategy to naive
3 - set collision strategy to qtree
4 - set collision strategy to parallel_qtree

Tools
B - set tool to ball_tool
L - set tool to line_tool
O - set tool to rectangle_tool
A - set tool to attraction_tool
)"
);

Simulation::Simulation(const sf::VideoMode vm, const bool full_screen) :
	video_mode(vm),
	full_screen(full_screen),
	world(Rectangle(0, 0, vm.width, vm.height)),
	ball_tool(&world),
	rectangle_tool(&world),
	line_tool(&world),
	attraction_tool(&world),
	debug_tool(&world),
	current_tool(&ball_tool)
{
	ball_tool.setBallsMass(1000);
	ball_tool.setBallsRadius(2);
	ball_tool.setBallsBounceFactor(1);
	ball_tool.setBallsPerDeploy(1000);

	loadAssets();
	view.reset(sf::FloatRect(0, 0, static_cast<float>(vm.width), static_cast<float>(vm.height)));
}

void Simulation::loadAssets()
{
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
}

Simulation::~Simulation() = default;

void Simulation::saveSimState(const std::string s) const {
	ofstream ofs(s, ios::binary);
	size_t n = world.BSpwn.balls.size();
	ofs.write((char *)&n, sizeof(n));
	for (int i = 0; i < n; i++) {
		ofs.write((char *)&world.BSpwn.balls[i], sizeof(Ball));
	}
	ofs.close();
}

void Simulation::loadSimState(const std::string s) {
	ifstream ifs(s, ios::binary);
	size_t n;
	ifs.read((char *)&n, sizeof(n));

	world.BSpwn.balls.clear();
	world.BSpwn.balls.resize(n);
	for (int i = 0; i < n; i++) {
		ifs.read((char *)&world.BSpwn.balls[i], sizeof(Ball));
	}
	ifs.close();
}

void Simulation::mainLoop() {
	sf::RenderWindow renderer;
	sf::Clock clock;

	if (full_screen) {
		renderer.create(video_mode, "Simulate Window", sf::Style::Fullscreen);
	}
	else {
		renderer.create(video_mode, "Simulate Window");
	}


	//view.setCenter(0,0); // TODO
    renderer.setView(view);

	while (renderer.isOpen())
	{
		// handle mouse and keyboard events
		clock.restart();
		[this, clock](sf::RenderWindow& renderer) { 
			eventLoop(renderer); 
			eventLoop_ms_time.newSample(clock.getElapsedTime().asMilliseconds()); 
		} (renderer);
		//eventLoop(renderer);
		

		// update world
		clock.restart();
		//std::thread update_thread([this, &clock]() {
			update();
			update_ms_time.newSample(clock.getElapsedTime().asMilliseconds());
		//});  

		// render graphics
		clock.restart();
		render(renderer);
		render_ms_time.newSample(clock.getElapsedTime().asMilliseconds());
	

		//update_thread.join();

		// update time
		time.update();

		if (quit) {
			renderer.close();
			quit = false;
		}
	}
}

void Simulation::update() {
	double dt = time.getDeltaT()*((this->paused) ? 0 : 1.f);
	world.update(dt);
	current_tool->update(dt);
}

void Simulation::eventLoop(sf::RenderWindow &renderer) {
	sf::Event event;
	while (renderer.pollEvent(event)) { //event loop
		switch (event.type) {
		case sf::Event::Closed:
			renderer.close();
			break;

		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseMoved:
			static sf::Event::MouseMoveEvent prev_mouse_move;
			current_tool->handleEvent(event, renderer);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
				view.move(static_cast<float>((prev_mouse_move.x - event.mouseMove.x)*scale),
					static_cast<float>((prev_mouse_move.y - event.mouseMove.y)*scale));
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

			case sf::Keyboard::T: // enable / disable rendering qtree
				draw_qtree = !draw_qtree;
				break;

			case sf::Keyboard::P: // pause
			case sf::Keyboard::Space: 
				paused = !paused;
				break;

			case sf::Keyboard::R: // pause
				this->world.BSpwn.balls.clear();
				break;

			case sf::Keyboard::G: // enable / disable gravity
				this->world.settings.gravity_forces = !this->world.settings.gravity_forces;
				break;

			case sf::Keyboard::B: // set tool to ball_tool
				this->setTool(Tool::BALL_TOOL);
				break;

			case sf::Keyboard::L: // set tool to line_tool
				this->setTool(Tool::LINE_TOOL);
				break;

			case sf::Keyboard::O: // set tool to rectangle_tool
				this->setTool(Tool::RECTANGLE_TOOL);
				break;

			case sf::Keyboard::A: // set tool to attraction_tool
				this->setTool(Tool::ATTRACTION_TOOL);
				break;

			case sf::Keyboard::F: // set tool to attraction_tool
				this->setTool(Tool::DEBUG_TOOL);
				break;

			case sf::Keyboard::Add: // increase simulation speed
				this->time.setTimeFactor(this->time.getTimeFactor() + 0.1f);
				break;

			case sf::Keyboard::Subtract: // decrase simulation speed
				this->time.setTimeFactor(this->time.getTimeFactor() - 0.1f);
				break;

			case sf::Keyboard::Num1: // set collision strategy to disable
				this->world.setCurrentBallCollissionStrategy(World::CollisionStrategyType::Disabled);
				break;

			case sf::Keyboard::Num2: // set collision strategy to naive
				this->world.setCurrentBallCollissionStrategy(World::CollisionStrategyType::Naive);
				break;

			case sf::Keyboard::Num3: // set collision strategy to qtree
				this->world.setCurrentBallCollissionStrategy(World::CollisionStrategyType::Qtree);
				break;

			case sf::Keyboard::Num4: // set collision strategy to parallel_qtree
				this->world.setCurrentBallCollissionStrategy(World::CollisionStrategyType::ParallelQtree);
				break;

			default:
				break;
			}
		default:
			break;
		}
	}
}

void Simulation::render(sf::RenderWindow &renderer) {
	renderer.clear();
	renderBalls(renderer, world.BSpwn.balls);
	renderRectangles(renderer, world.rectangles);
	renderLines(renderer, world.lines);
	current_tool->draw(renderer);
	if (this->draw_qtree) {
		renderQtree(renderer, world.BSpwn.balls);
	}

	if (this->print_debug) { // print debug info
		std::ostringstream ss;
		ss << "FPS: " << time.getAvgFps() << " ("<< static_cast<int>(time.getDeltaT() * 1000) << ")ms\n";
		ss << "Balls N: " << this->world.BSpwn.balls.size() << "\n";
		ss << "Ball Strategy: " << this->world.getCurrentBallCollissionStrategyName() << "\n";
		ss << "Gravity Forces: " << (this->world.settings.gravity_forces ? "enabled" : "disabled") << "\n";
		ss << "Simulation speed: " << this->time.getTimeFactor() << "\n";
		ss << "Current Tool: " << this->current_tool->getName() << "\n";
		ss << "Event loop time: " << this->eventLoop_ms_time.getAverage() << "ms\n";
		ss << "Update time: " << this->update_ms_time.getAverage() << "ms\n";
		ss << "Render time: " << this->render_ms_time.getAverage() << "ms\n";
		ss << "Tool Status: \n" << this->current_tool->getStatusAsString() << "\n";
		this->renderText(renderer, 10, 10, ss.str());
	}

	if (this->print_help) { // print help message
		this->renderText(renderer, 10, 10, HELP_MSG);
	}
	renderer.display();
}

void Simulation::setTool(const int id) {
	switch (id) {
	case Tool::BALL_TOOL:
		current_tool = &ball_tool;
		break;

	case Tool::RECTANGLE_TOOL:
		current_tool = &rectangle_tool;
		break;

	case Tool::LINE_TOOL:
		current_tool = &line_tool;
		break;

	case Tool::ATTRACTION_TOOL:
		current_tool = &attraction_tool;
		break;

	case Tool::DEBUG_TOOL:
		current_tool = &debug_tool;
		break;
	}
}

void Simulation::renderBalls(sf::RenderWindow &renderer, const std::vector<Ball>& balls) {
	sf::CircleShape circle;
	for (const Ball &ball : balls) {
		if (!Utils::isBallVisable(view, ball)) continue; // skip ball if is not on screen
		float x = static_cast<float>(ball.position.x);
		float y = static_cast<float>(ball.position.y);
		float r = static_cast<float>(ball.r);
		circle.setTexture(&this->ball_textures[ball.texture_id]);
		circle.setRadius(r);
		circle.setPosition(x - r, y - r);
		circle.setFillColor(ball.color);
		renderer.draw(circle);
	}
}

void Simulation::renderRectangles(sf::RenderWindow &renderer, const std::vector<Rectangle> &Rectangles) {
	sf::RectangleShape rect;
	for (const Rectangle &rectangle : Rectangles) {
		rect.setSize(sf::Vector2f(static_cast<float>(rectangle.rect.w), static_cast<float>(rectangle.rect.h)));
		rect.setPosition(static_cast<float>(rectangle.rect.x), static_cast<float>(rectangle.rect.y));
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineColor(rectangle.color);
		rect.setOutlineThickness(10.0f);
		//rect.getOutlineColor
		renderer.draw(rect);
	}
}

void Simulation::renderLines(sf::RenderWindow &renderer, const std::vector<Line> &Lines) {
	sf::RectangleShape rect;
	for (const Line &line : Lines) {
		rect.setSize(sf::Vector2f(static_cast<float>(line.length), static_cast<float>(line.width)));
		rect.setPosition(static_cast<float>(line.p1.x), static_cast<float>(line.p1.y));
		rect.setRotation(static_cast<float>(line.angle * 180.0f / M_PI));
		rect.setFillColor(line.color);
		renderer.draw(rect);
	}
}

void Simulation::renderText(sf::RenderWindow &renderer, const double x, const double y, const std::string s) {
	sf::View prev_state;
	prev_state = renderer.getView();
	renderer.setView(renderer.getDefaultView());

	sf::Text text;

	text.setFont(font);
	text.setString(s);
	text.setOutlineColor(sf::Color::Black);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(20);
	text.setPosition(static_cast<float>(x), static_cast<float>(y));
	renderer.draw(text);

	renderer.setView(prev_state);
}

void renderNode(sf::RenderWindow& renderer, const tml::node<double, const Ball*>* node) {
	sf::RectangleShape rect;
	rect.setPosition(static_cast<float>(node->x), static_cast<float>(node->y));
	rect.setSize(sf::Vector2f(static_cast<float>(node->x1 - node->x), static_cast<float>(node->y1 - node->y)));
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(2);
	renderer.draw(rect);
}

void renderTree(sf::RenderWindow& renderer, const tml::node<double, const Ball*>* node) {
	if (node != NULL && !node->leaf) {
		renderNode(renderer, node);
		for (int i = 0; i < 4; i++) {
			renderTree(renderer, node->child[i]);
		}
	}
}

void Simulation::renderQtree(sf::RenderWindow& renderer, const std::vector<Ball>& balls) {

	tml::qtree<double, const Ball*> tree(this->world.bbox.rect.x, this->world.bbox.rect.y, this->world.bbox.rect.x + this->world.bbox.rect.w, this->world.bbox.rect.y + this->world.bbox.rect.h);
	int fail_cnt = 0;

	int balls_n = balls.size();
	// build tree
	for (int i = 0; i < balls_n - 1; i++) {
		if (!tree.add_node(balls[i].position.x, balls[i].position.y, &balls[i])) {
			fail_cnt += 1;
		}
	}

	const tml::node<double, const Ball*>* root = tree.getRoot();

	renderTree(renderer, root);
}