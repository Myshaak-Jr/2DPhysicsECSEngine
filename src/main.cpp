#include "game/game.h"

int main(int argc, char** argv) {
	(void*)argc;
	(void*)argv;

	std::unique_ptr<Game> game = nullptr;

	try {
		game.reset(new Game());

		game->run();
	}
	catch (const std::exception& e) {
		SDL_Log("Error: %s", e.what());
		return 1;
	}

	return 1;
}