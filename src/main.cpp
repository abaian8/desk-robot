#include <Arduino.h>

// Onboard RGB LED on many ESP32-S3 DevKitC-1 boards is on GPIO 38
#define RGB_PIN 38 
#define BRIGHTNESS 30 // Set brightness (0-255)

void setup() {
  // neopixelWrite does not require pinMode configuration
}

void loop() {
  // Red
  neopixelWrite(RGB_PIN, BRIGHTNESS, 0, 0);
  delay(1000);

  // Green
  neopixelWrite(RGB_PIN, 0, BRIGHTNESS, 0);
  delay(1000);

  // Blue
  neopixelWrite(RGB_PIN, 0, 0, BRIGHTNESS);
  delay(1000);

  // Off
  neopixelWrite(RGB_PIN, 0, 0, 0);
  delay(1000);
}