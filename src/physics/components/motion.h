#pragma once

#include <glm/glm.hpp>

namespace components {
	struct linearMotion {
		glm::vec2 acceleration;
		glm::vec2 velocity;

		float invMass;

		inline linearMotion(float mass = 1.0f, glm::vec2 acceleration = glm::vec2(), glm::vec2 velocity = glm::vec2())
			: acceleration(acceleration), velocity(velocity), invMass(1.0f/mass)
		{}
	};

	struct angularMotion {
		float acceleration;
		float velocity;

		float invI;

		inline angularMotion(float I = 1.0f, float acceleration = 0.0f, float velocity = 0.0f)
			: acceleration(acceleration), velocity(velocity), invI(1.0f/I)
		{}
	};
}