#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "../common/ecsTypes.h"

struct factories {
	/*
	 * @brief Creates an entity with from the ball template.
	 * Full component list: position, rotation, linearMotion, angularMotion, circleGeometry
	 * @param registry A shared pointer to registry to create the ball in
	 * @param pos The default position of the ball
	 * @param rot The default rotation of the ball
	 * @param radius The raduis of the ball
	 * @param mass The mass of the ball
	 */
	static ecsTypes::entity createBall(const std::shared_ptr<ecsTypes::registry>& registry, glm::vec2 position, float rot, float radius, float mass);

	/*
	 * @brief Creates an entity with from the box template.
	 * Full component list: position, rotation, linearMotion, angularMotion, boxGeometry
	 * @param registry A shared pointer to registry to create the box in
	 * @param pos The default position of the box
	 * @param rot The default rotation of the box
	 * @param w The width of the box
	 * @param h The height of the box
	 * @param mass The mass of the box
	 */
	static ecsTypes::entity createBox(const std::shared_ptr<ecsTypes::registry>& registry, glm::vec2 pos, float rot, float w, float h, float mass);
};