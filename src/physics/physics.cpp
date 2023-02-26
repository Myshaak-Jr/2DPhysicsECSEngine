#include "physics.h"
#include <SDL.h>
#include <glm/gtx/rotate_vector.hpp>

#include "state.h"

#include "units.h"

#include "../graphics/state.h"

using namespace physics;
using namespace physics::units;

Physics::Physics(const std::shared_ptr<ecsTypes::registry>& registry, float pixelsPerMeter, glm::vec2 gravity) : registry(registry) {
	state::pixelsPerMeter = pixelsPerMeter;
	state::gravity = gravity;

	force = std::make_unique<Force>(registry);
};

// Systems
void Physics::update() const {
	// compute dt
	static long long int timePreviousFrame;
	float dt = (SDL_GetTicks64() - timePreviousFrame) / 1000.0f;
	// clamp delta time
	if (dt > (1000 / 60) / 500.0f) {
		dt = (1000 / 60) / 500.0f;
	}
	timePreviousFrame = SDL_GetTicks64();

	force->applyKeyboardPush();

	integrateLinear(dt);
	integrateAngular(dt);


	// JUST FOR NOW, THIS IS SO BAD AND UGLY
	for (auto [entity, pos, motion, circle] : registry->view<components::position, components::linearMotion, const components::circleGeometry>().each()) {
		if (pos.x - circle.radius < 0.0f) {
			pos.x = circle.radius;
			motion.velocity.x *= -0.5;
		}
		else if (pos.x + circle.radius > graphics::state::width) {
			pos.x = graphics::state::width - circle.radius;
			motion.velocity.x *= -0.5;
		}
		if (pos.y - circle.radius < 0.0f) {
			pos.y = circle.radius;
			motion.velocity.y *= -0.5;
		}
		else if (pos.y + circle.radius > graphics::state::height) {
			pos.y = graphics::state::height - circle.radius;
			motion.velocity.y *= -0.5;
		}
	}
}

void Physics::integrateLinear(float dt) const {
	auto view = registry->view<components::position, components::linearMotion>();

	for (auto [entity, pos, motion] : view.each()) {
		motion.velocity += motion.acceleration * dt;
		pos += motion.velocity * dt;

		motion.acceleration = glm::vec2();
	}
}

void Physics::integrateAngular(float dt) const {
	auto view = registry->view<components::rotation, components::angularMotion>();

	for (auto [entity, rot, motion] : view.each()) {
		motion.velocity += motion.acceleration * dt;
		rot.angle += motion.velocity * dt;

		motion.acceleration = 0.0f;
	}
}