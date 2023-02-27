#include "force.h"
#include <SDL.h>

#include "../units.h"

#include "../components.h"

using namespace physics;
using namespace physics::units;

Force::Force(const std::shared_ptr<ecsTypes::registry>& registry) : registry(registry) {}

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

	for (auto [entity, motion] : registry->view<components::linearMotion>().each()) {
		motion.sumForces += pushForce;
	}

	for (auto [entity, motion] : registry->view<components::angularMotion>().each()) {
		motion.sumTorque += pushTorque;
	}
}
