#include "physics.h"
#include <SDL.h>
#include <glm/gtx/rotate_vector.hpp>

Physics::Physics(const std::shared_ptr<ecsTypes::registry>& registry, float pixelsPerMeter, glm::vec2 gravity) : registry(registry), pixelsPerMeter(pixelsPerMeter), gravity(gravity) {

};


// Systems
void Physics::applyForce(ecsTypes::entity entity, glm::vec2 force) const {
	if (!registry->any_of<components::linearMotion>(entity)) {
		SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Tried to apply force to entity without linearMotion component");
		return;
	};

	auto& linearMotion = registry->get<components::linearMotion>(entity);

	linearMotion.acceleration += force * linearMotion.invMass;
}

void Physics::applyTorque(ecsTypes::entity entity, float torque) const {
	if (!registry->any_of<components::angularMotion>(entity)) {
		SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Tried to apply force to entity without angularMotion component");
		return;
	};

	auto& angularMotion = registry->get<components::angularMotion>(entity);

	angularMotion.acceleration += torque * angularMotion.invI;
	SDL_Log("%.3f", angularMotion.invI);
}

void Physics::update() const {
	// compute dt
	static long long int timePreviousFrame;
	float dt = (SDL_GetTicks64() - timePreviousFrame) / 1000.0f;
	// clamp delta time
	if (dt > (1000 / 60) / 500.0f) {
		dt = (1000 / 60) / 500.0f;
	}
	timePreviousFrame = SDL_GetTicks64();

	applyKeyboardPush();

	integrateLinear(dt);
	integrateAngular(dt);
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

void Physics::applyKeyboardPush() const {
	auto keyState = SDL_GetKeyboardState(NULL);

	glm::vec2 pushForce = glm::vec2();
	float pushTorque = 0.0f;

	if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_A]) {
		pushForce.x -= 50.0f * pixelsPerMeter;
	}
	if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_D]) {
		pushForce.x += 50.0f * pixelsPerMeter;
	}
	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
		pushForce.y -= 50.0f * pixelsPerMeter;
	}
	if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
		pushForce.y += 50.0f * pixelsPerMeter;
	}
	if (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_Q]) {
		pushTorque -= 500.0f;
	}
	if (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_E]) {
		pushTorque += 500.0f;
	}

	for (auto entity : registry->view<components::linearMotion>()) {
		applyForce(entity, pushForce);
	}

	for (auto entity : registry->view<components::angularMotion>()) {
		applyTorque(entity, pushTorque);
	}
}
