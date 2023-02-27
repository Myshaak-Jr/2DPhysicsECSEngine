#pragma once

#include <glm/glm.hpp>
#include "../units.h"

using namespace physics::units;

namespace components {
	struct linearMotion {
		glm::vec2 acceleration;
		glm::vec2 velocity;
		glm::vec2 sumForces;

		inline linearMotion(glm::vec2 sumForces = glm::vec2(), glm::vec2 acceleration = glm::vec2(), glm::vec2 velocity = glm::vec2())
			: acceleration(acceleration), velocity(velocity), sumForces(sumForces)
		{}
	};

	struct mass {
		float invMass;
		inline mass(float mass = 1_kg) : invMass(1.0f / mass) {}
	};

	struct angularMotion {
		float acceleration;
		float velocity;
		float sumTorque;

		float invI;

		inline angularMotion(float I = 1.0f, float sumTorque = 0.0f, float acceleration = 0.0f, float velocity = 0.0f)
			: acceleration(acceleration), velocity(velocity), invI(1.0f/I), sumTorque(sumTorque)
		{}
	};
}