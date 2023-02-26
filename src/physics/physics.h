#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <glm/glm.hpp>

#include "../common/ecsTypes.h"

#include "../common/components.h"
#include "components.h"

class Physics {
public:
	Physics(const std::shared_ptr<ecsTypes::registry>& registry, float pixelsPerMeter, glm::vec2 gravity);
	~Physics() = default;

	/* High-Level force application on an entity.
	 * Early returns if entity does not have linearMotion component
	 */
	void applyForce(ecsTypes::entity entity, glm::vec2 force) const;

	/* High-Level torque application on an entity
	 * Early returns if entity does not have angularMotion component
	 */
	void applyTorque(ecsTypes::entity entity, float torque) const;

	/*
	 * The update system which runs all the other physics-related systems
	 */
	void update() const;

private:
	float pixelsPerMeter;
	glm::vec2 gravity;

	std::shared_ptr<ecsTypes::registry> registry;

	// PHYSICS-RELATED SYSTEMS

	/* Euler integration method system for linear motion
	 */
	void integrateLinear(float dt) const;

	/* Euler integration method system for angular motion
	 */
	void integrateAngular(float dt) const;

	// TODO: comment
	void applyKeyboardPush() const;
};