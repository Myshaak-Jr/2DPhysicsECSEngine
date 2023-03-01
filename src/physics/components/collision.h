#pragma once

namespace components {
	struct collision {
		bool colliding;
		// Coefficient of restitution (elasticity)
		float restitution;

		inline collision(float restitution, bool colliding = false) : colliding(colliding), restitution(restitution) {}
	};
}