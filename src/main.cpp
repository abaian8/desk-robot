#include <Arduino.h>
#include "bluetooth/bluetooth.h"
#include "drive/drive.h"
#include "ears/ears.h"

// ─────────────────────────────────────────────────────────────────────────────
// BLE TEST BUILD — Bluetooth + minimal hardware bring-up.
// Drive is initialised first so the H-bridge IN pins are driven LOW/braked
// before anything else runs (otherwise floating IN pins make a track spin
// uncontrollably at boot).
// ─────────────────────────────────────────────────────────────────────────────

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

// ── Serial drive calibration ─────────────────────────────────────────────────
// Arrow keys in a terminal arrive as a 3-byte ANSI sequence: ESC '[' <code>.
//   Up = 'A', Down = 'B', Right = 'C', Left = 'D'.
// Space (or 's') = stop. Single-byte keys ('w','a','s','d') also work as a
// fallback in case your serial monitor doesn't forward escape sequences.
static void handleDriveKeys() {
    static uint8_t esc = 0;   // 0 idle, 1 saw ESC, 2 saw ESC '['

    while (Serial.available() > 0) {
        int c = Serial.read();
        if (c < 0) break;

        if (esc == 1) {
            esc = (c == '[') ? 2 : 0;
            continue;
        }
        if (esc == 2) {
            esc = 0;
            switch (c) {
                case 'A': Drive::forward();   Serial.println("[DRIVE] forward");  break;
                case 'B': Drive::backward();  Serial.println("[DRIVE] backward"); break;
                case 'C': Drive::turnRight(); Serial.println("[DRIVE] right");    break;
                case 'D': Drive::turnLeft();  Serial.println("[DRIVE] left");     break;
                default:  break;
            }
            continue;
        }

        switch (c) {
            case 0x1B: esc = 1; break;                                                  // ESC
            case ' ':
            case 's':
            case 'S':  Drive::stop();      Serial.println("[DRIVE] stop");     break;
            case 'w':
            case 'W':  Drive::forward();   Serial.println("[DRIVE] forward");  break;
            case 'x':
            case 'X':  Drive::backward();  Serial.println("[DRIVE] backward"); break;
            case 'a':
            case 'A':  Drive::turnLeft();  Serial.println("[DRIVE] left");     break;
            case 'd':
            case 'D':  Drive::turnRight(); Serial.println("[DRIVE] right");    break;
            default:   break;
        }
    }
}

void setup() {
    // Kill the motors FIRST so a floating-pin track doesn't keep spinning
    // while the rest of setup() runs.
    Drive::init();
    Drive::stop();

    Serial.begin(115200);
    Serial.println("[BOOT] BLE test starting...");

    Ears::init();
  
    Bluetooth::init(onBleReceive);

    Serial.println("[BOOT] Advertising as 'DeskRobot'. Open nRF Connect and scan.");
    Serial.println("[DRIVE] Arrow keys to drive  |  space/s = stop  |  fallback: w/x/a/d");
}

void loop() {
    Bluetooth::update();
    handleDriveKeys();
    delay(10);
}
