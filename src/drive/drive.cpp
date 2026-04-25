#include "drive.h"
#include "../config/pins.h"
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────────────
// On/off mode: ENA/ENB are jumpered HIGH on the L298N, so we control each
// motor purely via its two IN pins.
//   IN1=H, IN2=L → forward
//   IN1=L, IN2=H → backward
//   IN1=L, IN2=L → coast (off)
//   IN1=H, IN2=H → active brake  (not used here)
// ─────────────────────────────────────────────────────────────────────────────

static uint8_t _speed = DRIVE_DEFAULT_SPEED;   // stored but currently inert

static void _setMotor(uint8_t pinIn1, uint8_t pinIn2, int8_t dir) {
    if (dir > 0) {
        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn2, LOW);
    } else if (dir < 0) {
        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn2, HIGH);
    } else {
        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn2, LOW);
    }
}

void Drive::init() {
    pinMode(PIN_MOTOR_L_IN1, OUTPUT);
    pinMode(PIN_MOTOR_L_IN2, OUTPUT);
    pinMode(PIN_MOTOR_R_IN1, OUTPUT);
    pinMode(PIN_MOTOR_R_IN2, OUTPUT);
    stop();
}

void Drive::move(Direction dir) {
    switch (dir) {
        case Direction::FORWARD:  forward();   break;
        case Direction::BACKWARD: backward();  break;
        case Direction::LEFT:     turnLeft();  break;
        case Direction::RIGHT:    turnRight(); break;
        case Direction::STOP:     stop();      break;
    }
}

void Drive::forward() {
    _setMotor(PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2,  1);
    _setMotor(PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2,  1);
}

void Drive::backward() {
    _setMotor(PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, -1);
    _setMotor(PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, -1);
}

void Drive::turnLeft() {
    // Tank spin: left reverse, right forward.
    _setMotor(PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, -1);
    _setMotor(PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2,  1);
}

void Drive::turnRight() {
    // Tank spin: left forward, right reverse.
    _setMotor(PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2,  1);
    _setMotor(PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, -1);
}

void Drive::stop() {
    _setMotor(PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, 0);
    _setMotor(PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, 0);
}

void Drive::setSpeed(uint8_t speed) {
    _speed = speed;
}

uint8_t Drive::getSpeed() {
    return _speed;
}
