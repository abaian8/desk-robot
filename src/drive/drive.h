#pragma once
#include <stdint.h>

// ─────────────────────────────────────────────────────────────────────────────
// DriveModule — controls the two GA25-370 DC motor tracks via an H-bridge.
//
// Movement model:
//   FORWARD  — both tracks forward at full speed
//   BACKWARD — both tracks backward at full speed
//   LEFT     — left track slow/reverse, right track forward (pivot turn)
//   RIGHT    — right track slow/reverse, left track forward (pivot turn)
//   STOP     — both tracks off
// ─────────────────────────────────────────────────────────────────────────────

namespace Drive {

    enum class Direction { FORWARD, BACKWARD, LEFT, RIGHT, STOP };

    // Must be called once in setup().
    void init();

    // Move in the given direction using the current speed setting.
    void move(Direction dir);

    // Convenience wrappers.
    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    void stop();

    // Set base speed for both tracks (0–255).  Persists until changed.
    void setSpeed(uint8_t speed);

    uint8_t getSpeed();

} // namespace Drive
