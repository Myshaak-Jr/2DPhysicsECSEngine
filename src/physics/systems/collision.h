#pragma once

#include <memory>
#include "../../common/ecsTypes.h"

#include <glm/glm.hpp>

namespace physics {
	class Collision {
	public:
		Collision(const std::shared_ptr<ecsTypes::registry>& registry);
		~Collision() = default;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
	};
}
