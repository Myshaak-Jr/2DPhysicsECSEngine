#pragma once

#include <entt/entt.hpp>
#include "../graphics/graphics.h"
#include "../physics/physics.h"
#include "../common/ecsTypes.h"

class Game {
public:
	Game(int fps = 60);
	~Game();

	void run();

private:
	bool running;
	int fps;

	std::shared_ptr<ecsTypes::registry> registry;

	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Physics> physics;

	void setup();
	void update();
	void draw();
};