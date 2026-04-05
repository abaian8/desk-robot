#pragma once

// ─────────────────────────────────────────────────────────────────────────────
// CommandHandler — parses incoming BLE strings and dispatches them to the
// correct subsystem module.
//
// This is the single place where the text protocol is translated into robot
// actions.  Add new commands here as the robot gains new capabilities.
//
// Current protocol:
//   "FWD"         → Drive forward
//   "BCK"         → Drive backward
//   "LFT"         → Turn left
//   "RGT"         → Turn right
//   "STP"         → Stop motors
//   "SPD:<0-255>" → Set motor speed   e.g. "SPD:180"
//   "EMO:<name>"  → Set emotion       e.g. "EMO:HAPPY"
//
// Unrecognised commands are logged to Serial and ignored gracefully.
// ─────────────────────────────────────────────────────────────────────────────

namespace CommandHandler {

    // Must be called once in setup(), after all subsystem modules are ready.
    void init();

    // Parse and dispatch a single null-terminated command string received over BLE.
    // Returns a short response string that should be sent back to the app (e.g. "OK:FWD").
    // The returned pointer is valid until the next call to handle().
    const char* handle(const char* command);

} // namespace CommandHandler
