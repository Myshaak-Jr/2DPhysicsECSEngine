#pragma once

#include <memory>
#include "../../common/ecsTypes.h"

#include <glm/glm.hpp>

namespace physics {
	class Force {
	public:
		Force(const std::shared_ptr<ecsTypes::registry>& registry);
		~Force() = default;

		/* Calculates the force and torque from the keyboard input
		 */
		void applyKeyboardPush() const;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
	};
}