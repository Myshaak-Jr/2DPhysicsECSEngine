#include "physics.h"
#include <SDL.h>
#include <glm/gtx/rotate_vector.hpp>

#include "state.h"

#include "units.h"

#include "../graphics/state.h"

using namespace physics;
using namespace physics::units;

Physics::Physics(const std::shared_ptr<ecsTypes::registry>& registry, const std::shared_ptr<ecsTypes::dispatcher>& dispatcher) : registry(registry), dispatcher(dispatcher) {
	force = std::make_unique<Force>(registry);
	collision = std::make_unique<Collision>(registry, dispatcher);
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

	//force->applyKeyboardPush();
	
	//for (auto [entity, motion] : registry->view<components::linearMotion>().each()) {
	//	motion.sumForces += glm::vec2(20_N, 0_N);
	//}

	//applyGravity();

	integrateLinear(dt);
	integrateAngular(dt);

	collision->update();

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
		motion.acceleration += motion.sumForces * motion.invMass;

		motion.velocity += motion.acceleration * dt;
		pos += motion.velocity * dt;

		motion.sumForces = glm::vec2();
		motion.acceleration = glm::vec2();
	}
}

void Physics::integrateAngular(float dt) const {
	auto view = registry->view<components::rotation, components::angularMotion>();

	for (auto [entity, rot, motion] : view.each()) {
		motion.acceleration += motion.sumTorque * motion.invI;

		motion.velocity += motion.acceleration * dt;
		rot.angle += motion.velocity * dt;

		motion.sumTorque = 0.0f;
		motion.acceleration = 0.0f;
	}
}

void Physics::applyGravity() const {
	auto view = registry->view<components::linearMotion>();

	for (auto [entity, motion] : view.each()) {
		motion.acceleration += state::gravity;
	}
}