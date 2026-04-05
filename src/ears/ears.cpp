#include "ears.h"
#include "../config/pins.h"
#include <ESP32Servo.h>

// ─────────────────────────────────────────────────────────────────────────────
// Internal state
// ─────────────────────────────────────────────────────────────────────────────

static Servo _leftServo;
static Servo _rightServo;

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void Ears::init() {
    // ESP32Servo allocates a LEDC channel automatically.
    _leftServo.attach(PIN_SERVO_EAR_LEFT);
    _rightServo.attach(PIN_SERVO_EAR_RIGHT);
    neutral();
}

void Ears::setPosition(Position pos) {
    switch (pos) {
        case Position::NEUTRAL: neutral(); break;
        case Position::UP:      up();      break;
        case Position::DOWN:    down();    break;
    }
}

void Ears::setLeftAngle(uint8_t degrees) {
    _leftServo.write(degrees);
}

void Ears::setRightAngle(uint8_t degrees) {
    _rightServo.write(degrees);
}

void Ears::setBothAngles(uint8_t degrees) {
    _leftServo.write(degrees);
    _rightServo.write(degrees);
}

void Ears::neutral() {
    setBothAngles(EAR_ANGLE_NEUTRAL);
}

void Ears::up() {
    setBothAngles(EAR_ANGLE_UP);
}

void Ears::down() {
    setBothAngles(EAR_ANGLE_DOWN);
}
