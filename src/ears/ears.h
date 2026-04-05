#pragma once
#include <stdint.h>

// ─────────────────────────────────────────────────────────────────────────────
// EarsModule — controls the two servo-driven "ear" structures.
//
// Ears reflect the robot's emotional state:
//   UP      — perky / alert / happy
//   DOWN    — sad / sleepy
//   NEUTRAL — default resting position
//
// Individual ear angles can also be set directly (0–180°).
// ─────────────────────────────────────────────────────────────────────────────

namespace Ears {

    enum class Position { NEUTRAL, UP, DOWN };

    // Must be called once in setup().
    void init();

    // Move both ears to a named preset.
    void setPosition(Position pos);

    // Set individual ear angles (0–180 degrees).
    void setLeftAngle(uint8_t degrees);
    void setRightAngle(uint8_t degrees);
    void setBothAngles(uint8_t degrees);

    // Convenience presets.
    void neutral();
    void up();
    void down();

} // namespace Ears
