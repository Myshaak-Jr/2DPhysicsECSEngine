#pragma once

#include "state.h"
#include <glm/glm.hpp>

// TODO: make the units calculated at use time to support changin the pixelsPerMeter variable over time

#define UNIT_DEF_GEN(unit, scale) \
        constexpr float operator""##unit##(long double unit) { \
            return static_cast<float>(unit) * (scale); \
        } \
        constexpr float operator""##unit##(unsigned long long unit) { \
            return static_cast<float>(unit) * (scale); \
        }


namespace physics {
    namespace units {
        // LENGTH
        //---------------------------------------------------------
        // Meters
        UNIT_DEF_GEN(_m, state::pixelsPerMeter);

        // Centimeters
        UNIT_DEF_GEN(_cm, 0.01f * state::pixelsPerMeter);

        // millimeters
        UNIT_DEF_GEN(_mm, 0.001f * state::pixelsPerMeter);

        // Pixels
        UNIT_DEF_GEN(_px, 1.0f);


        // ANGLE
        //---------------------------------------------------------
        // Radians
        UNIT_DEF_GEN(_rad, 1.0f);

        // Degrees
        UNIT_DEF_GEN(_deg, glm::radians(1.0f));

        // TIME
        //---------------------------------------------------------
        // Seconds
        UNIT_DEF_GEN(_s, 1.0f);

        // MASS
        //---------------------------------------------------------
        // Kilograms
        UNIT_DEF_GEN(_kg, 1.0f);

        // FORCE
        //---------------------------------------------------------
        // Newtons
        UNIT_DEF_GEN(_N, 1_kg * 1_m / (1_s * 1_s));

        // Kilonewtons
        UNIT_DEF_GEN(_kN, 1000.0f * 1_kg * 1_m / (1_s * 1_s));

        // TORQUE
        //---------------------------------------------------------
        // Newtonmeter
        
        UNIT_DEF_GEN(_Nm, 1_m * 1_N);
    }
}
