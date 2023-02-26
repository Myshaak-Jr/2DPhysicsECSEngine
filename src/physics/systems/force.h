#pragma once

#include <memory>
#include "../../common/ecsTypes.h"

#include <glm/glm.hpp>

namespace physics {
	class Force {
	public:
		Force(const std::shared_ptr<ecsTypes::registry>& registry);
		~Force() = default;

		/* High-Level force application on an entity.
		 * Early returns if entity does not have linearMotion component
		 */
		void applyForce(ecsTypes::entity entity, glm::vec2 force) const;

		/* High-Level torque application on an entity
		 * Early returns if entity does not have angularMotion component
		 */
		void applyTorque(ecsTypes::entity entity, float torque) const;

		/* Calculates the force and torque from the keyboard input
		 */
		void applyKeyboardPush() const;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
	};
}