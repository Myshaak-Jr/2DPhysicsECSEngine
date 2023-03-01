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
{
	// Subsribe to events
	dispatcher->sink<events::collision>().connect<&Collision::resolveCollision>(this);
}

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

	dispatcher->enqueue(enterData);
}

void Collision::enqueueExitCollision(ecsTypes::entity entity, ecsTypes::entity other) const {
	auto& collision = registry->get<components::collision>(entity);
	if (!collision.colliding) return;

	collision.colliding = false;

	events::collisionExit exitData{};
	exitData.entity = entity;
	exitData.other = other;

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

void Collision::resolvePenetration(const events::collision& e) const {
	const auto& mass1 = registry->get<components::mass>(e.e1);
	const auto& mass2 = registry->get<components::mass>(e.e2);

	float k = e.depth / (mass1.invMass + mass2.invMass);

	if (!mass1.isStatic) {
		auto& pos1 = registry->get<components::position>(e.e1);
		pos1 -= e.normal * (k * mass1.invMass);
	}
	if (!mass2.isStatic) {
		auto& pos2 = registry->get<components::position>(e.e2);
		pos2 += e.normal * (k * mass2.invMass);
	}
}

void Collision::resolveCollision(const events::collision& e) const {
	const auto& mass1 = registry->get<components::mass>(e.e1);
	const auto& mass2 = registry->get<components::mass>(e.e2);

	if (mass1.isStatic && mass2.isStatic) return;

	resolvePenetration(e);

	// Get restitution
	const auto& collision1 = registry->get<components::collision>(e.e1);
	const auto& collision2 = registry->get<components::collision>(e.e2);

	float restitution = glm::min(collision1.restitution, collision2.restitution);
	//SDL_Log("Restitution %.5f.", restitution);

	// Get velocities
	glm::vec2 vel1 = glm::vec2();
	if (!mass1.isStatic) {
		vel1 = registry->get<components::linearMotion>(e.e1).velocity;
	}
	glm::vec2 vel2 = glm::vec2();
	if (!mass2.isStatic) {
		vel2 = registry->get<components::linearMotion>(e.e2).velocity;
	}

	// Calculate the impulse
	const glm::vec2 vrel = vel1 - vel2;
	float vrelDotNormal = glm::dot(vrel, e.normal);
	const glm::vec2 impulseDirection = e.normal;
	float impulseLength = -(1 + restitution) * vrelDotNormal / (mass1.invMass + mass2.invMass);

	const glm::vec2 impulse = impulseDirection * impulseLength;

	// Apply the impulse
	if (!mass1.isStatic) {
		registry->get<components::linearMotion>(e.e1).velocity += impulse * mass1.invMass;
	}
	if (!mass2.isStatic) {
		registry->get<components::linearMotion>(e.e2).velocity -= impulse * mass2.invMass;
	}
}