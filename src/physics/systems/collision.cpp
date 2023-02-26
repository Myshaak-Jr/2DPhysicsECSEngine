#include "collision.h"

#include "../units.h"

#include "../../common/components.h"
#include "../components.h"

using namespace physics;
using namespace physics::units;

Collision::Collision(const std::shared_ptr<ecsTypes::registry>& registry) : registry(registry) {}
