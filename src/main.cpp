#include <Arduino.h>
#include "bluetooth/bluetooth.h"

// ─────────────────────────────────────────────────────────────────────────────
// BLE TEST BUILD — only Bluetooth is active.
// All other modules (Face, Ears, Drive, Emotions, Commands) are commented out
// so you can flash and test BLE in isolation without any hardware connected.
//
// To restore the full build: uncomment the includes and setup/loop code below.
// ─────────────────────────────────────────────────────────────────────────────

// #include "drive/drive.h"
// #include "ears/ears.h"
// #include "face/face.h"
// #include "emotions/emotions.h"
// #include "commands/commands.h"

static void onBleReceive(const char* raw) {
    // Just print whatever the app sends — full command dispatch comes later.
    Serial.printf("[BLE RX] \"%s\"\n", raw);

    // Echo it straight back to the app.
    char ack[72];
    snprintf(ack, sizeof(ack), "OK:%s", raw);
    Bluetooth::send(ack);
}

void setup() {
    Serial.begin(115200);
    Serial.println("[BOOT] BLE test starting...");

    Bluetooth::init(onBleReceive);

    Serial.println("[BOOT] Advertising as 'DeskRobot'. Open nRF Connect and scan.");
}

void loop() {
    Bluetooth::update();
    delay(10);
}
