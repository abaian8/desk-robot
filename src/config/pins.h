#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// Pin Configuration — ESP32-S3-DevKitC-1-N8R8
// Update these to match your actual wiring before building.
// ─────────────────────────────────────────────────────────────────────────────

// ── Drive Motors (GA25-370 via H-Bridge, e.g. L298N) ─────────────────────────
// Left track
#define PIN_MOTOR_L_PWM     14   // ENA — PWM speed control
#define PIN_MOTOR_L_IN1     12   // IN1 — direction
#define PIN_MOTOR_L_IN2     13   // IN2 — direction

// Right track
#define PIN_MOTOR_R_PWM     11   // ENB — PWM speed control
#define PIN_MOTOR_R_IN1      9   // IN3 — direction
#define PIN_MOTOR_R_IN2     10   // IN4 — direction

// ── Ear Servos ────────────────────────────────────────────────────────────────
#define PIN_SERVO_EAR_LEFT   4   // Left  ear servo signal
#define PIN_SERVO_EAR_RIGHT  5   // Right ear servo signal

// ── TFT Display (SPI — configured separately in User_Setup.h) ────────────────
// Pins are defined in lib/TFT_eSPI/User_Setup.h.
// Listed here for reference only — do NOT redefine them.
// #define TFT_MOSI  11  (example — check your User_Setup.h)
// #define TFT_SCLK  12
// #define TFT_CS    10
// #define TFT_DC     9
// #define TFT_RST    8
// #define TFT_BL     3   // Optional backlight PWM

// ── Motor PWM Channels (ESP32 LEDC) ──────────────────────────────────────────
#define MOTOR_L_PWM_CHANNEL   0
#define MOTOR_R_PWM_CHANNEL   1
#define MOTOR_PWM_FREQ      1000   // Hz
#define MOTOR_PWM_RESOLUTION   8   // bits → 0–255

// ── Drive Defaults ────────────────────────────────────────────────────────────
#define DRIVE_DEFAULT_SPEED  200   // 0–255
#define DRIVE_TURN_SPEED     150   // speed used for the slower track while turning

// ── Ear Servo Angle Presets (degrees) ─────────────────────────────────────────
#define EAR_ANGLE_NEUTRAL    90
#define EAR_ANGLE_UP        130
#define EAR_ANGLE_DOWN       50

// ── BLE ───────────────────────────────────────────────────────────────────────
#define BLE_DEVICE_NAME     "DeskRobot"
