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

	struct angularMotion {
		float acceleration;
		float velocity;
		float sumTorque;

		inline angularMotion(float sumTorque = 0.0f, float acceleration = 0.0f, float velocity = 0.0f)
			: acceleration(acceleration), velocity(velocity), sumTorque(sumTorque)
		{}
	};

	struct mass {
		float invMass;
		float invI;
		bool isStatic;

		inline mass(float mass, float I = 1.0f, bool isStatic = false) : invMass(isStatic ? 0.0f : 1.0f / mass), invI(isStatic ? 0.0f : 1.0f / I), isStatic(isStatic) {}
	};
}