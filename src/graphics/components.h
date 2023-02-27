#pragma once

#include <cstdint>

namespace components {
	struct color {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		inline color() : r(0), g(0), b(0), a(255) {}
		inline color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
		inline color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
	};
}