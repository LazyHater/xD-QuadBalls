#include "simulation.hpp"
#include <sstream>

Simulation::Simulation(sf::VideoMode vm, bool full_screen) : video_mode(vm), full_screen(full_screen) {
	environment = new Environment(Rectangle(0, 0, vm.width, vm.height));
	environment->settings.ball_to_ball_collisions = true;

	ball_tool = new BallTool(environment);
	ball_tool->setBallsMass(1);
	ball_tool->setBallsPerDeploy(100);
	ball_tool->setBallsRadius(10);
	ball_tool->setBallsBounceFactor(1);
	rectangle_tool = new RectangleTool(environment);
	line_tool = new LineTool(environment);
	current_tool = ball_tool;

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

	ball_tool->setBallsTexturesNumber(textures_n);

	view.reset(sf::FloatRect(0, 0, vm.width, vm.height));
}

Simulation::~Simulation() {
	delete environment;
	delete ball_tool;
	delete rectangle_tool;
	delete line_tool;
}

void Simulation::process() {
	sf::RenderWindow renderer;
	sf::VideoMode::getFullscreenModes()[0];
	if (full_screen)
		renderer.create(video_mode, "Simulate Window", sf::Style::Fullscreen);
	else
		renderer.create(video_mode, "Simulate Window");

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
				current_tool->update(event, renderer);
				break;
			case sf::Event::MouseButtonReleased:
				current_tool->update(event, renderer);
				break;
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
					paused = !paused;
					break;

				case sf::Keyboard::P: // pause
					paused = !paused;
					break;

				case sf::Keyboard::C: // enable / disable collisions
					this->environment->settings.ball_to_ball_collisions = !this->environment->settings.ball_to_ball_collisions;
					break;
				}
			}
			break;
		}

		environment->update(time.get_delta_t()*((this->paused) ? 0 : 1.f));//if paused then make 0 delta t

		renderer.clear();
		drawBalls(renderer, environment->BSpwn.balls);
		drawRectangles(renderer, environment->rectangles);
		drawLines(renderer, environment->lines);
		current_tool->draw(renderer);

		if (this->debug) { // print debug info
			std::ostringstream ss;
			ss << "FPS: " << time.get_current_FPS() << "\n";
			ss << "Balls N: " << this->environment->BSpwn.balls.size() << "\n";
			ss << "Ball Strategy: " << this->environment->current_ball_strategy->name;
			this->drawText(renderer, 0, 0, ss.str());
		}
		renderer.display();

		time.update();

		if (quit) {
			renderer.close();
			quit = false;
		}
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