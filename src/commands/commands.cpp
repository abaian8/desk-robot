#include "commands.h"
#include "../drive/drive.h"
#include "../emotions/emotions.h"
#include "../face/face.h"
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

// ─────────────────────────────────────────────────────────────────────────────
// Internal response buffer — reused on every call to handle().
// ─────────────────────────────────────────────────────────────────────────────

static char _response[72];

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

// Build a response string and return it.
static const char* _respond(const char* prefix, const char* body = "") {
    snprintf(_response, sizeof(_response), "%s%s", prefix, body);
    return _response;
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void CommandHandler::init() {
    // Nothing to initialise for now — subsystems are already up by the time
    // this is called.  Kept here so setup() stays consistent.
    Serial.println("[CMD] Command handler ready.");
}

const char* CommandHandler::handle(const char* cmd) {
    // Always log the raw string to serial — useful during development.
    Serial.printf("[BLE RX] \"%s\"\n", cmd);

    // ── Movement ─────────────────────────────────────────────────────────────
    if (strcmp(cmd, "FWD") == 0) {
        Drive::forward();
        return _respond("OK:FWD");
    }
    if (strcmp(cmd, "BCK") == 0) {
        Drive::backward();
        return _respond("OK:BCK");
    }
    if (strcmp(cmd, "LFT") == 0) {
        Drive::turnLeft();
        return _respond("OK:LFT");
    }
    if (strcmp(cmd, "RGT") == 0) {
        Drive::turnRight();
        return _respond("OK:RGT");
    }
    if (strcmp(cmd, "STP") == 0) {
        Drive::stop();
        return _respond("OK:STP");
    }

    // ── Speed: "SPD:<0-255>" ─────────────────────────────────────────────────
    if (strncmp(cmd, "SPD:", 4) == 0) {
        uint8_t spd = (uint8_t)constrain(atoi(cmd + 4), 0, 255);
        Drive::setSpeed(spd);
        return _respond("OK:", cmd);
    }

    // ── Emotion: "EMO:<name>" ────────────────────────────────────────────────
    if (strncmp(cmd, "EMO:", 4) == 0) {
        Face::Emotion e = EmotionManager::fromString(cmd + 4);
        EmotionManager::set(e);
        return _respond("OK:", cmd);
    }

    // ── Unknown ──────────────────────────────────────────────────────────────
    Serial.printf("[CMD] Unknown command: \"%s\"\n", cmd);
    return _respond("ERR:UNKNOWN:", cmd);
}
