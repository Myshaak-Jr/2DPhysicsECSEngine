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
	
	for (auto [entity, motion] : registry->view<components::linearMotion>().each()) {
		motion.sumForces += glm::vec2(2_N, 0_N);
	}

	applyGravity();

	integrateLinear(dt);
	integrateAngular(dt);

	collision->update();

	// JUST FOR NOW, THIS IS SO BAD AND UGLY
	for (auto [entity, pos, motion, circle, collision] : registry->view<components::position, components::linearMotion, const components::circleGeometry, const components::collision>().each()) {
		if (pos.x - circle.radius < 0.0f) {
			pos.x = circle.radius;
			motion.velocity.x *= -collision.restitution;
		}
		else if (pos.x + circle.radius > graphics::state::width) {
			pos.x = graphics::state::width - circle.radius;
			motion.velocity.x *= -collision.restitution;
		}
		if (pos.y - circle.radius < 0.0f) {
			pos.y = circle.radius;
			motion.velocity.y *= -collision.restitution;
		}
		else if (pos.y + circle.radius > graphics::state::height) {
			pos.y = graphics::state::height - circle.radius;
			motion.velocity.y *= -collision.restitution;
		}
	}
}

void Physics::integrateLinear(float dt) const {
	auto view = registry->view<components::position, components::linearMotion, const components::mass>();

	for (auto [entity, pos, motion, mass] : view.each()) {
		if (mass.isStatic) continue;

		motion.acceleration = motion.sumForces * mass.invMass;

		motion.velocity += motion.acceleration * dt;
		pos += motion.velocity * dt;

		motion.sumForces = glm::vec2();
	}
}

void Physics::integrateAngular(float dt) const {
	auto view = registry->view<components::rotation, components::angularMotion, const components::mass>();

	for (auto [entity, rot, motion, mass] : view.each()) {
		if (mass.isStatic) continue;

		motion.acceleration = motion.sumTorque * mass.invI;

		motion.velocity += motion.acceleration * dt;
		rot.angle += motion.velocity * dt;

		motion.sumTorque = 0.0f;
	}
}

void Physics::applyGravity() const {
	auto view = registry->view<components::linearMotion, const components::mass>();

	for (auto [entity, motion, mass] : view.each()) {
		if (mass.isStatic) continue;
		motion.sumForces += state::gravity / mass.invMass;
	}
}