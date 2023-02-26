#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace components {
	struct circleGeometry {
		float radius;

		inline circleGeometry(float radius = 0.0f) : radius(radius) {}
	};

	struct polygonGeometry {
		std::vector<glm::vec2> vertices;

		inline polygonGeometry() = default;
		inline polygonGeometry(std::vector<glm::vec2> vertices) : vertices(vertices) {}
	};

	struct boxGeometry : polygonGeometry {
		float w;
		float h;

		inline boxGeometry(float w, float h) : w(w), h(h) {
			vertices.emplace_back(-w / 2.0f, -h / 2.0f);
			vertices.emplace_back(+w / 2.0f, -h / 2.0f);
			vertices.emplace_back(+w / 2.0f, +h / 2.0f);
			vertices.emplace_back(-w / 2.0f, +h / 2.0f);
		}
	};
}