#pragma once

namespace components {
	struct collision {
		bool colliding;

		inline collision(bool colliding = false) : colliding(colliding) {}
	};
}