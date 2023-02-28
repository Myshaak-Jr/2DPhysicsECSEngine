#include "factories.h"

#include "../common/components.h"
#include "../graphics/components.h"
#include "../physics/components.h"

using namespace components;

ecsTypes::entity factories::createBall(const std::shared_ptr<ecsTypes::registry>& registry, glm::vec2 pos, float rot, float radius, float mass, bool isStatic) {
	auto entity = registry->create();
	registry->emplace<position>(entity, pos);
	registry->emplace<rotation>(entity, rot);

	if (!isStatic) {
		registry->emplace<linearMotion>(entity);
		registry->emplace<angularMotion>(entity);
	}
	registry->emplace<components::mass>(entity, mass, 0.5f * radius * radius * mass, isStatic);

	registry->emplace<circleGeometry>(entity, radius);

	registry->emplace<color>(entity, 255, 0, 0);
	registry->emplace<collision>(entity);

	return entity;
}

ecsTypes::entity factories::createBox(const std::shared_ptr<ecsTypes::registry>& registry, glm::vec2 pos, float rot, float w, float h, float mass, bool isStatic) {
	auto entity = registry->create();
	registry->emplace<position>(entity, pos);
	registry->emplace<rotation>(entity, rot);

	registry->emplace<linearMotion>(entity);
	registry->emplace<angularMotion>(entity);
	registry->emplace<components::mass>(entity, mass, (1.0f / 12.0f) * (w * w + h * h) * mass, isStatic);

	registry->emplace<boxGeometry>(entity, w, h);
	registry->emplace<collision>(entity);

	registry->emplace<color>(entity, 255, 0, 0);

	return entity;
}