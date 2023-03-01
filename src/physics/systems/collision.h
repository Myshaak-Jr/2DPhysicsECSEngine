#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../../common/ecsTypes.h"

#include "collisionEvent.h"
#include "../components.h"

namespace physics {
	class Collision {
	public:
		Collision(const std::shared_ptr<ecsTypes::registry>& registry, const std::shared_ptr<ecsTypes::dispatcher>& dispatcher);
		~Collision() = default;

		/* Checkes and resolves collisions
		 */
		void update() const;

	private:
		std::shared_ptr<ecsTypes::registry> registry;
		std::shared_ptr<ecsTypes::dispatcher> dispatcher;

		/* Checkes and resolves every circle-circle collisions
		 */
		void circleCircleCollisionLoop() const;

		/* Check for circle-circle collisions between e1 and e2
		 */
		bool circleCircleCollisionCheck(events::collision& data) const;
		
		// TODO: bool PolygonPolygonCollisionCheck(ecsTypes::entity e1, ecsTypes::entity e2) const;
		// TODO: bool CirclePolygonCollisionCheck(ecsTypes::entity e1, ecsTypes::entity e2) const;

		/* Enqueues enter collision event if entity entered collision
		 */
		void enqueueEnterCollision(ecsTypes::entity entity, ecsTypes::entity other, const events::collision data) const;

		/* Enqueues exit collision event if entity exited collision
		 */
		void enqueueExitCollision(ecsTypes::entity entity, ecsTypes::entity other) const;

		/* Resolves collision between entities
		 * Listener for collision events
		 * @param data The event data
		 */
		void resolveCollision(const events::collision& data) const;

		/* Shifts the entities from collision
		 */
		void resolvePenetration(const events::collision& data) const;
	};
}
