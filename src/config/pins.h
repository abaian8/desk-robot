#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// Pin Configuration — ESP32-S3-DevKitC-1-N8R8
// Update these to match your actual wiring before building.
// ─────────────────────────────────────────────────────────────────────────────

// ── Drive Motors (GA25-370 via H-Bridge, e.g. L298N) ─────────────────────────
// ENA / ENB are NOT driven from the MCU right now — the on-board jumpers on
// the L298N are left in place so ENA/ENB stay tied HIGH. Motors are full-on
// whenever IN1 != IN2, full-off when IN1 == IN2. To restore PWM speed control
// later: pull the jumpers, wire ENA/ENB to the GPIOs below, and re-enable the
// LEDC code in drive.cpp.
//
// #define PIN_MOTOR_L_PWM     14   // ENA — PWM speed control (unused for now)
// #define PIN_MOTOR_R_PWM     11   // ENB — PWM speed control (unused for now)

// Left track
#define PIN_MOTOR_L_IN1     47   // IN1 — direction
#define PIN_MOTOR_L_IN2     48   // IN2 — direction
                                 // (was GPIO 19 = USB_D-; that pad is driven by
                                 //  the on-chip USB Serial/JTAG controller from
                                 //  reset and made the left track spin at boot.)

// Right track
#define PIN_MOTOR_R_IN1     18   // IN3 — direction
                                 // (was GPIO 20 = USB_D+; same reason as above.)
#define PIN_MOTOR_R_IN2     19   // IN4 — direction

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
// Unused while ENA/ENB are jumpered HIGH. Kept here for when PWM is re-enabled.
// #define MOTOR_L_PWM_CHANNEL   0
// #define MOTOR_R_PWM_CHANNEL   1
// #define MOTOR_PWM_FREQ      1000   // Hz
// #define MOTOR_PWM_RESOLUTION   8   // bits → 0–255

// ── Drive Defaults ────────────────────────────────────────────────────────────
// Speed values are stored by Drive::setSpeed() but have no effect until ENA/ENB
// PWM is wired back up.
#define DRIVE_DEFAULT_SPEED  255   // 0–255
#define DRIVE_TURN_SPEED     255   // unused in on/off mode

// ── Ear Servo Angle Presets (degrees) ─────────────────────────────────────────
#define EAR_ANGLE_NEUTRAL    90
#define EAR_ANGLE_UP        130
#define EAR_ANGLE_DOWN       50

// ── BLE ───────────────────────────────────────────────────────────────────────
#define BLE_DEVICE_NAME     "DeskRobot"
