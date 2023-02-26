#pragma once

#include <SDL.h>
#include <memory>
#include <entt/entt.hpp>
#include <cstdint>
#include <glm/glm.hpp>

#include "../common/ecsTypes.h"

/* Handles the window and rendering
 */
struct Graphics {
	Graphics(const std::shared_ptr<ecsTypes::registry>& registry, uint32_t bgColor);
	~Graphics();

	/*
	 * Returns the width of the window
	 */
	int width();

	/*
	 * Returns the height of the window
	 */
	int height();

	/*
	 * Return a galm::vec2 to a center of the window 
	 */
	glm::vec2 center();

	/*
	 * The draw system which runs all the other graphics-related systems
	 */
	void draw();

private:
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;

	std::shared_ptr<ecsTypes::registry> registry;

	uint32_t bgColor;

	// Graphical function
	
	/* Draw a polygon rotated by angle `angle` from `vertices` and a dot at coordinates `center`
	 * Vertices are transformed to a world space during the process
	 * @param center the center to draw from
	 * @param vertices vector of vertices in local space
	 * @param angle the angle to rotate by
	 * @param color the color to draw with
	 */
	void drawPolygonAndCenter(glm::vec2 center, const std::vector<glm::vec2>& vertices, float angle, uint32_t color);

	/* Draw a circle and radius inside rotated by angle `angle` at coordinates `center`
	 * @param center the center to draw from
	 * @param radius the radius of the circle
	 * @param angle the angle to rotate by
	 * @param color the color to draw with
	 */
	void drawCircleAndRadius(glm::vec2 center, float radius, float angle, uint32_t color);

	// GRAPHICS-RELATED SYSTEMS
	
	/* Draws the bounding boxes of entities with some geometry
	 * An exceptional system that is both graphics and
	 * physics related, but it fits here more
	 */
	void drawPhysicsCircleGeometry();
	void drawPhysicsPolygonGeometry();
	void drawPhysicsBoxGeometry();
};

