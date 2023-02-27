#include "game.h"

#include <SDL.h>
#include <glm/gtc/constants.hpp>

#include "factories.h"

#include "../physics/units.h"

using namespace physics::units;

Game::Game(int fps) : running(false), fps(fps) {
	SDL_Log("Game constructor called!");

	registry = std::make_shared<ecsTypes::registry>();
	dispatcher = std::make_shared<ecsTypes::dispatcher>();
	graphics = std::make_unique<graphics::Graphics>(registry, dispatcher, 0xFF000000);
	physics = std::make_unique<physics::Physics>(registry, dispatcher);
}

Game::~Game() {
	SDL_Log("Game destructor called!");
}

void Game::run() {
	running = true;

	setup();

	while (running) {
		// set fps
		static long long int timePreviousFrame;
		int expectedMillisecs = (1000 / fps);
		long long int timeToWait = expectedMillisecs - (SDL_GetTicks64() - timePreviousFrame);
		if (timeToWait > 0) {
			SDL_Delay(timeToWait);
		}
		timePreviousFrame = SDL_GetTicks64();

		update();
	}
}

void Game::setup() {
	// Init physics state
	physics::state::pixelsPerMeter = 100_px;
	physics::state::gravity = glm::vec2(0.0f, 9.81_m / (1_s * 1_s));

	// Create entities
	player = factories::createBall(registry, glm::vec2(100_cm, 100_cm), 0_rad, 100_cm, 1_kg);
	factories::createBall(registry, glm::vec2(500_cm, 100_cm), 0_rad, 50_cm, 1_kg);
}

void Game::update() {
	// Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				running = false;
			break;
		case SDL_MOUSEMOTION:
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			auto& pos = registry->get<components::position>(player);
			pos.x = x;
			pos.y = y;
		}

			break;
		default:
			break;
		}
	}

	// Update Systems
	physics->update();
	graphics->update();
}
