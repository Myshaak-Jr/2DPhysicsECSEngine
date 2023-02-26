#pragma once

#include <glm/glm.hpp>

namespace components {
	struct position : glm::vec2 {
		inline position() : glm::vec2() {}
		inline position(float scalar) : glm::vec2(scalar) {}
		inline position(float x, float y) : glm::vec2(x, y) {}
		inline position(const glm::vec2& v) : glm::vec2(v) {}
	};

	struct rotation {
		float angle;

		inline rotation(float angle = 0.0f) : angle(angle) {}
	};
}