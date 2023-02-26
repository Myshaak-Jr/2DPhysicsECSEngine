#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <glm/glm.hpp>

#include "systems/force.h"

#include "../common/ecsTypes.h"

#include "../common/components.h"
#include "components.h"

namespace physics {
	class Physics {
	public:
		Physics(const std::shared_ptr<ecsTypes::registry>& registry, float pixelsPerMeter, glm::vec2 gravity);
		~Physics() = default;

		/*
		 * The update system which runs all the other physics-related systems
		 */
		void update() const;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
		std::unique_ptr<Force> force;


		// PHYSICS-RELATED SYSTEMS

		/* Euler integration method system for linear motion
			*/
		void integrateLinear(float dt) const;

		/* Euler integration method system for angular motion
		 */
		void integrateAngular(float dt) const;
	};
}