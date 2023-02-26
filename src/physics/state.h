#pragma once

#include <glm/glm.hpp>

namespace physics {
	struct state {
		inline static float pixelsPerMeter;
		inline static glm::vec2 gravity;
	};
}