#pragma once
#include <glm/glm.hpp>

#include "../../common/ecsTypes.h"

namespace events {
	struct collision {
		ecsTypes::entity e1;
		ecsTypes::entity e2;

		glm::vec2 start;
		glm::vec2 end;

		glm::vec2 normal;
		float depth;
	};

	struct collisionEnter {
		ecsTypes::entity entity;
		ecsTypes::entity other;

		glm::vec2 start;
		glm::vec2 end;

		glm::vec2 normal;
		float depth;
	};

	struct collisionExit {
		ecsTypes::entity entity;
		ecsTypes::entity other;
	};
}