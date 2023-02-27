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

	ecsTypes::entity player;

	std::shared_ptr<ecsTypes::registry> registry;
	std::shared_ptr<ecsTypes::dispatcher> dispatcher;

	std::unique_ptr<graphics::Graphics> graphics;
	std::unique_ptr<physics::Physics> physics;

	void setup();
	void update();
};