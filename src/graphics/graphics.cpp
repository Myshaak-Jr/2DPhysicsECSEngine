#include "graphics.h"
#include <stdexcept>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

#include "components.h"
#include "../common/components.h"
#include "../physics/components/geometry.h"

#include "state.h"

using namespace graphics;

Graphics::Graphics(const std::shared_ptr<ecsTypes::registry>& registry, uint32_t bgColor) : registry(registry), window(nullptr, &SDL_DestroyWindow), renderer(nullptr, &SDL_DestroyRenderer), bgColor(bgColor){
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
		throw std::runtime_error("Failed to initialize SDL2");
	}

	// Create the window
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	state::width = display_mode.w;
	state::height = display_mode.h;

	window.reset(SDL_CreateWindow(
		"Game",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		state::width,
		state::height,
		SDL_WINDOW_BORDERLESS
	));
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		throw std::runtime_error("Failed to create window");
	}

	// Create the renderer
	renderer.reset(SDL_CreateRenderer(
		window.get(),
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	));
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		throw std::runtime_error("Failed to create renderer");
	}
}

Graphics::~Graphics() {
	SDL_Quit();
}

glm::vec2 Graphics::center() {
	int w, h;
	SDL_GetWindowSize(window.get(), &w, &h);
	return glm::vec2(w/2.0f, h/2.0f);
}


// Graphical functions
void Graphics::drawPolygonAndCenter(glm::vec2 center, const std::vector<glm::vec2>& vertices, float angle, uint32_t color) {
	// transform to world space
	std::vector<glm::vec2> worldSpace(vertices.size());
	std::transform(vertices.begin(), vertices.end(), worldSpace.begin(), [angle, center](const glm::vec2& vertex) {
		return glm::rotate(vertex, angle) + center;
	});
	
	// draw
	for (int i = 0; i < worldSpace.size(); i++) {
		int currIndex = i;
		int nextIndex = (i + 1) % worldSpace.size();
		lineColor(renderer.get(), worldSpace[currIndex].x, worldSpace[currIndex].y, worldSpace[nextIndex].x, worldSpace[nextIndex].y, color);
	}
	filledCircleColor(renderer.get(), center.x, center.y, 1, color);
}

void Graphics::drawCircleAndRadius(glm::vec2 center, float radius, float angle, uint32_t color) {
	circleColor(renderer.get(), center.x, center.y, radius, color);
	lineColor(renderer.get(), center.x, center.y, center.x + glm::cos(angle) * radius, center.y + glm::sin(angle) * radius, color);
}

// Systems
void Graphics::draw() {
	SDL_SetRenderDrawColor(renderer.get(), bgColor >> 16, bgColor >> 8, bgColor, 255);
	SDL_RenderClear(renderer.get());

	drawPhysicsCircleGeometry();
	drawPhysicsPolygonGeometry();
	drawPhysicsBoxGeometry();

	SDL_RenderPresent(renderer.get());
}

void Graphics::drawPhysicsCircleGeometry() {
	auto view = registry->view<const components::position, const components::rotation, const components::circleGeometry>();

	for (auto [entity, pos, rot, circle] : view.each()) {
		drawCircleAndRadius(pos, circle.radius, rot.angle, 0xFF0000FF);
	}
}
void Graphics::drawPhysicsPolygonGeometry() {
	auto view = registry->view<const components::position, const components::rotation, const components::polygonGeometry>();
	
	for (auto [entity, pos, rot, polygon] : view.each()) {
		drawPolygonAndCenter(pos, polygon.vertices, rot.angle, 0xFF0000FF);
	}
}
void Graphics::drawPhysicsBoxGeometry() {
	auto view = registry->view<const components::position, const  components::rotation, const components::boxGeometry>();
	
	for (auto [entity, pos, rot, polygon] : view.each()) {
		drawPolygonAndCenter(pos, polygon.vertices, rot.angle, 0xFF0000FF);
	}
}