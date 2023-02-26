#include "game.h"

#include <SDL.h>
#include <glm/gtc/constants.hpp>

#include "factories.h"

Game::Game(int fps) : running(false), fps(fps) {
	SDL_Log("Game constructor called!");

	registry = std::make_shared<ecsTypes::registry>();
	graphics = std::make_unique<Graphics>(registry, 0xFF056263);;
	physics = std::make_unique<Physics>(registry, 50.f, glm::vec2(0.0f, 9.8f));
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
		draw();
	}
}

void Game::setup() {
	factories::createBall(registry, graphics->center(), glm::pi<float>(), 50.0f, 8.1f);
	factories::createBall(registry, graphics->center() + glm::vec2(150.0f, 0.0f), glm::pi<float>() / 4.0f, 25.0f, 0.6f);
	factories::createBall(registry, graphics->center() + glm::vec2(-150.0f, 0.0f), 0.0f, 75.0f, 3.7f);

	factories::createBox(registry, graphics->center() + glm::vec2(0.0f, 150.0f), glm::pi<float>() / 8.0f, 150.0f, 75.0f, 1.2f);
	factories::createBox(registry, graphics->center() + glm::vec2(0.0f, -250.0f), glm::pi<float>() * 0.95870f, 42.0f, 212.0f, 5.4f);
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
		default:
			break;
		}
	}

	// Update Systems
	physics->update();
}

void Game::draw() {
	graphics->draw();
}