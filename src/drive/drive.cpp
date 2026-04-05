#include "drive.h"
#include "../config/pins.h"
#include <Arduino.h>

// ─────────────────────────────────────────────────────────────────────────────
// Internal helpers
// ─────────────────────────────────────────────────────────────────────────────

static uint8_t _speed = DRIVE_DEFAULT_SPEED;

// Set a single motor: positive duty = forward, negative = backward, 0 = brake.
static void _setMotor(uint8_t pwmChannel, uint8_t pinIn1, uint8_t pinIn2, int16_t duty) {
    if (duty > 0) {
        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn2, LOW);
        ledcWrite(pwmChannel, (uint8_t)duty);
    } else if (duty < 0) {
        digitalWrite(pinIn1, LOW);
        digitalWrite(pinIn2, HIGH);
        ledcWrite(pwmChannel, (uint8_t)(-duty));
    } else {
        // Brake: both IN pins HIGH (short-circuit brake on L298N)
        digitalWrite(pinIn1, HIGH);
        digitalWrite(pinIn2, HIGH);
        ledcWrite(pwmChannel, 0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void Drive::init() {
    // Direction pins
    pinMode(PIN_MOTOR_L_IN1, OUTPUT);
    pinMode(PIN_MOTOR_L_IN2, OUTPUT);
    pinMode(PIN_MOTOR_R_IN1, OUTPUT);
    pinMode(PIN_MOTOR_R_IN2, OUTPUT);

    // PWM channels (ESP32 LEDC)
    ledcSetup(MOTOR_L_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcAttachPin(PIN_MOTOR_L_PWM, MOTOR_L_PWM_CHANNEL);

    ledcSetup(MOTOR_R_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_PWM_RESOLUTION);
    ledcAttachPin(PIN_MOTOR_R_PWM, MOTOR_R_PWM_CHANNEL);

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
    _setMotor(MOTOR_L_PWM_CHANNEL, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2,  _speed);
    _setMotor(MOTOR_R_PWM_CHANNEL, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2,  _speed);
}

void Drive::backward() {
    _setMotor(MOTOR_L_PWM_CHANNEL, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, -_speed);
    _setMotor(MOTOR_R_PWM_CHANNEL, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, -_speed);
}

void Drive::turnLeft() {
    // Pivot: right track forward, left track backward
    _setMotor(MOTOR_L_PWM_CHANNEL, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, -DRIVE_TURN_SPEED);
    _setMotor(MOTOR_R_PWM_CHANNEL, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2,  _speed);
}

void Drive::turnRight() {
    // Pivot: left track forward, right track backward
    _setMotor(MOTOR_L_PWM_CHANNEL, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2,  _speed);
    _setMotor(MOTOR_R_PWM_CHANNEL, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, -DRIVE_TURN_SPEED);
}

void Drive::stop() {
    _setMotor(MOTOR_L_PWM_CHANNEL, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2, 0);
    _setMotor(MOTOR_R_PWM_CHANNEL, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2, 0);
}

void Drive::setSpeed(uint8_t speed) {
    _speed = speed;
}

uint8_t Drive::getSpeed() {
    return _speed;
}
