#include <Arduino.h>

#include "drive/drive.h"
#include "ears/ears.h"
#include "face/face.h"
#include "bluetooth/bluetooth.h"
#include "emotions/emotions.h"

// ─────────────────────────────────────────────────────────────────────────────
// BLE command handler
// Called from Bluetooth::update() on the main loop whenever a complete
// command string arrives from the remote control app.
//
// Command reference (see bluetooth.h for full protocol docs):
//   "FWD" / "BCK" / "LFT" / "RGT" / "STP"
//   "SPD:<0-255>"
//   "EMO:<name>"
// ─────────────────────────────────────────────────────────────────────────────

static void onCommand(const char* cmd) {
    Serial.printf("[CMD] %s\n", cmd);

    if      (strcmp(cmd, "FWD") == 0) { Drive::forward();   }
    else if (strcmp(cmd, "BCK") == 0) { Drive::backward();  }
    else if (strcmp(cmd, "LFT") == 0) { Drive::turnLeft();  }
    else if (strcmp(cmd, "RGT") == 0) { Drive::turnRight(); }
    else if (strcmp(cmd, "STP") == 0) { Drive::stop();      }

    else if (strncmp(cmd, "SPD:", 4) == 0) {
        uint8_t spd = (uint8_t)atoi(cmd + 4);
        Drive::setSpeed(spd);
    }

    else if (strncmp(cmd, "EMO:", 4) == 0) {
        Face::Emotion e = EmotionManager::fromString(cmd + 4);
        EmotionManager::set(e);
    }

    // Echo acknowledgement back to app.
    char ack[72];
    snprintf(ack, sizeof(ack), "OK:%s", cmd);
    Bluetooth::send(ack);
}

// ─────────────────────────────────────────────────────────────────────────────
// Setup
// ─────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    Serial.println("[BOOT] Desk Robot starting...");

    Face::init();
    Face::showStatus("Booting...");

    Ears::init();
    Drive::init();
    EmotionManager::init();

    Bluetooth::init(onCommand);

    Face::showStatus("Ready — waiting for BT");
    Serial.println("[BOOT] Ready.");
}

// ─────────────────────────────────────────────────────────────────────────────
// Loop
// ─────────────────────────────────────────────────────────────────────────────

void loop() {
    // Process any incoming BLE commands.
    Bluetooth::update();

    // Animate the face (blinking, etc.).
    Face::update();

    // Update the status bar whenever BT connection state changes.
    static bool _wasConnected = false;
    bool nowConnected = Bluetooth::isConnected();
    if (nowConnected != _wasConnected) {
        _wasConnected = nowConnected;
        Face::showStatus(nowConnected ? "BT Connected" : "Waiting for BT");
    }

    delay(10);  // Yield — keeps BLE stack happy.
}
