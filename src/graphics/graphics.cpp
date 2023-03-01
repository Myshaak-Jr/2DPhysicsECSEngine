#include "graphics.h"
#include <stdexcept>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

#include "../physics/units.h"

#include "components.h"
#include "../common/components.h"
#include "../physics/components.h"

#include "state.h"

using namespace graphics;
using namespace physics::units;

Graphics::Graphics(const std::shared_ptr<ecsTypes::registry>& registry, const std::shared_ptr<ecsTypes::dispatcher>& dispatcher, uint32_t bgColor) : registry(registry), globalDispatcher(dispatcher), window(nullptr, &SDL_DestroyWindow), renderer(nullptr, &SDL_DestroyRenderer), bgColor(bgColor){
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

	// Set listeners
	localDispatcher = std::make_unique<ecsTypes::dispatcher>();
	
	dispatcher->sink<events::collision>().connect<&Graphics::onEvent<events::collision>>(this);
	localDispatcher->sink<events::collision>().connect<&Graphics::drawDebugCollisionInfo>(this);
	dispatcher->sink<events::collisionEnter>().connect<&Graphics::setGreen>(this);
	dispatcher->sink<events::collisionExit>().connect<&Graphics::resetGreen>(this);
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
void Graphics::update() {
	draw();
}

void Graphics::draw() {
	SDL_RenderClear(renderer.get());

	drawPhysicsCircleGeometry();
	drawPhysicsPolygonGeometry();
	drawDebugMotionInfo();
	localDispatcher->update<events::collision>();

	SDL_RenderPresent(renderer.get());

	SDL_SetRenderDrawColor(renderer.get(), bgColor >> 16, bgColor >> 8, bgColor, 255);
}

void Graphics::setGreen(const events::collisionEnter& e) const {
	registry->get<components::color>(e.entity).g = 255;
}

void Graphics::resetGreen(const events::collisionExit& e) const {
	registry->get<components::color>(e.entity).g = 0;
}
void Graphics::drawDebugMotionInfo() const {
	auto view = registry->view<const components::position, const components::linearMotion>();
	
	for (auto [entity, pos, motion] : view.each()) {
		lineColor(renderer.get(), pos.x, pos.y, pos.x + motion.velocity.x, pos.y + motion.velocity.y, 0xFF00FFFF);
		//lineColor(renderer.get(), pos.x + motion.velocity.x, pos.y + motion.velocity.y, pos.x + motion.velocity.x + motion.acceleration.x, pos.y + motion.velocity.y + motion.acceleration.y, 0xFFFFFFFF);
	}
}

void Graphics::drawDebugCollisionInfo(const events::collision& e) const {	
	filledCircleColor(renderer.get(), e.start.x, e.start.y, 3, 0xFFFF00FF);
	filledCircleColor(renderer.get(), e.end.x, e.end.y, 3, 0xFFFF00FF);
	lineColor(renderer.get(), e.start.x, e.start.y, e.start.x + e.normal.x * 10_cm, e.start.y + e.normal.y * 10_cm, 0xFFFF00FF);
}

void Graphics::drawPhysicsCircleGeometry() {
	auto view = registry->view<const components::position, const components::rotation, const components::circleGeometry, const components::color>();

	for (auto [entity, pos, rot, circle, color] : view.each()) {
		//drawCircleAndRadius(pos, circle.radius, rot.angle, color.a << 24 | color.b << 16 | color.g << 8 | color.r);
		filledCircleColor(renderer.get(), pos.x, pos.y, circle.radius, color.a << 24 | color.b << 16 | color.g << 8 | color.r);
	}
}
void Graphics::drawPhysicsPolygonGeometry() {
	auto view = registry->view<const components::position, const components::rotation, const components::color>();

	for (auto [entity, pos, rot, color] : view.each()) {
		std::vector<glm::vec2> vertices;
		if (registry->all_of<components::polygonGeometry>(entity)) {
			vertices = registry->get<components::polygonGeometry>(entity).vertices;
		}
		else if (registry->all_of<components::boxGeometry>(entity)) {
			vertices = registry->get<components::boxGeometry>(entity).vertices;
		}
		drawPolygonAndCenter(pos, vertices, rot.angle, color.a << 24 | color.b << 16 | color.g << 8 | color.r);
	}
}
