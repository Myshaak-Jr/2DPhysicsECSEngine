#include "collision.h"
#include <SDL.h>
#include <glm/gtx/norm.hpp>

#include "../units.h"

#include "../../common/components.h"
#include "../../graphics/components.h"

using namespace physics;
using namespace physics::units;

Collision::Collision(const std::shared_ptr<ecsTypes::registry>& registry, const std::shared_ptr<ecsTypes::dispatcher>& dispatcher) :
	registry(registry),
	dispatcher(dispatcher)
{}

void Collision::update() const {
	circleCircleCollisionLoop();

	dispatcher->update<events::collision>();
	dispatcher->update<events::collisionEnter>();
	dispatcher->update<events::collisionExit>();
}

void Collision::circleCircleCollisionLoop() const {
	auto view = registry->view<const components::position, const components::circleGeometry, components::collision>();

	for (auto it1 = view.begin(); it1 != view.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != view.end(); ++it2) {
			events::collision data{};
			data.e1 = *it1;
			data.e2 = *it2;
			
			if (!circleCircleCollisionCheck(data)) {
				enqueueExitCollision(data.e1, data.e2);
				enqueueExitCollision(data.e2, data.e1);

				continue;
			}

			enqueueEnterCollision(data.e1, data.e2, data);
			enqueueEnterCollision(data.e2, data.e1, data);

			dispatcher->enqueue(data);
		}
	}
}

void Collision::enqueueEnterCollision(ecsTypes::entity entity, ecsTypes::entity other, const events::collision data) const {
	auto& collision = registry->get<components::collision>(entity);
	if (collision.colliding) return;

	collision.colliding = true;

	events::collisionEnter enterData{};
	enterData.entity = entity;
	enterData.other = other;
	enterData.start = data.start;
	enterData.end = data.end;
	enterData.normal = data.normal;
	enterData.depth = data.depth;

	SDL_Log("Enqueued event Enter Collision!");

	dispatcher->enqueue(enterData);
}

void Collision::enqueueExitCollision(ecsTypes::entity entity, ecsTypes::entity other) const {
	auto& collision = registry->get<components::collision>(entity);
	if (!collision.colliding) return;

	collision.colliding = false;

	events::collisionExit exitData{};
	exitData.entity = entity;
	exitData.other = other;

	SDL_Log("Enqueued event Exit Collision!");

	dispatcher->enqueue(exitData);
}

bool Collision::circleCircleCollisionCheck(events::collision& data) const {
	const auto& pos1 = registry->get<components::position>(data.e1);
	const auto& circle1 = registry->get<components::circleGeometry>(data.e1);
	const auto& pos2 = registry->get<components::position>(data.e2);
	const auto& circle2 = registry->get<components::circleGeometry>(data.e2);

	// Detect collision
	const glm::vec2 diff = pos2 - pos1;
	const float sumRadius = circle1.radius + circle2.radius;

	bool isColliding = glm::length2(diff) <= (sumRadius * sumRadius);

	if (!isColliding) {
		return false;
	}
	
	// Populate the data
	data.normal = glm::normalize(diff);

	data.start = pos2 - data.normal * circle2.radius;
	data.end = pos1 + data.normal * circle1.radius;

	data.depth = glm::length(data.end - data.start);

	return true;
}
