#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <glm/glm.hpp>

#include "systems/force.h"
#include "systems/collision.h"

#include "../common/ecsTypes.h"

#include "../common/components.h"
#include "components.h"

namespace physics {
	class Physics {
	public:
		Physics(const std::shared_ptr<ecsTypes::registry>& registry, const std::shared_ptr<ecsTypes::dispatcher>& dispatcher);
		~Physics() = default;

		/*
		 * The update system which runs all the other physics-related systems
		 */
		void update() const;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
		std::shared_ptr<ecsTypes::dispatcher> dispatcher;
		std::unique_ptr<Force> force;
		std::unique_ptr<Collision> collision;


		// PHYSICS-RELATED SYSTEMS

		/* Euler integration method system for linear motion
			*/
		void integrateLinear(float dt) const;

		/* Euler integration method system for angular motion
		 */
		void integrateAngular(float dt) const;

		/* Applies gravity defined in physics::state to every
		 * entity with linearMotion
		 */
		void applyGravity() const;
	};
}