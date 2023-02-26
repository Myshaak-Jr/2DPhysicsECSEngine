#include "force.h"
#include <SDL.h>

#include "../units.h"

#include "../components.h"

using namespace physics;
using namespace physics::units;

Force::Force(const std::shared_ptr<ecsTypes::registry>& registry) : registry(registry) {}

void Force::applyForce(ecsTypes::entity entity, glm::vec2 force) const {
	if (!registry->any_of<components::linearMotion>(entity)) {
		SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Tried to apply force to entity without linearMotion component");
		return;
	};

	auto& linearMotion = registry->get<components::linearMotion>(entity);

	linearMotion.acceleration += force * linearMotion.invMass;
}

void Force::applyTorque(ecsTypes::entity entity, float torque) const {
	if (!registry->any_of<components::angularMotion>(entity)) {
		SDL_LogWarn(SDL_LOG_PRIORITY_WARN, "Tried to apply force to entity without angularMotion component");
		return;
	};

	auto& angularMotion = registry->get<components::angularMotion>(entity);

	angularMotion.acceleration += torque * angularMotion.invI;
}

void Force::applyKeyboardPush() const {
	auto keyState = SDL_GetKeyboardState(NULL);

	glm::vec2 pushForce = glm::vec2();
	float pushTorque = 0_Nm;

	if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_A]) {
		pushForce.x -= 50_N;
	}
	if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_D]) {
		pushForce.x += 50_N;
	}
	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
		pushForce.y -= 50_N;
	}
	if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
		pushForce.y += 50_N;
	}
	if (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_Q]) {
		pushTorque -= 25_Nm;
	}
	if (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_RCTRL] || keyState[SDL_SCANCODE_E]) {
		pushTorque += 25_Nm;
	}

	for (auto entity : registry->view<components::linearMotion>()) {
		applyForce(entity, pushForce);
	}

	for (auto entity : registry->view<components::angularMotion>()) {
		applyTorque(entity, pushTorque);
	}
}
