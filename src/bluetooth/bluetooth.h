#pragma once
#include <functional>

// ─────────────────────────────────────────────────────────────────────────────
// BluetoothModule — BLE GATT server for the desk-robot remote control app.
//
// Protocol (UTF-8 text over BLE, newline-terminated):
//
//   From app  → robot (Write characteristic):
//     "FWD"           Move forward  (button held)
//     "BCK"           Move backward
//     "LFT"           Turn left
//     "RGT"           Turn right
//     "STP"           Stop (button released / explicit stop)
//     "SPD:<0-255>"   Set motor speed  e.g. "SPD:180"
//     "EMO:<name>"    Trigger emotion  e.g. "EMO:HAPPY"
//
//   From robot → app (Notify characteristic):
//     "OK:<echo>"     Acknowledge last command
//     "STAT:<json>"   Periodic status update (speed, emotion, etc.)
//
// UUIDs follow the standard Nordic UART Service (NUS) convention so the
// Flutter app can use any generic BLE UART library.
// ─────────────────────────────────────────────────────────────────────────────

namespace Bluetooth {

    // Callback type: invoked on the main loop when a complete command arrives.
    using CommandCallback = std::function<void(const char* command)>;

    // Must be called once in setup().  Pass a handler for incoming commands.
    void init(CommandCallback onCommand);

    // Call from loop() to process any queued BLE events.
    void update();

    // Send a UTF-8 string to the connected app via the Notify characteristic.
    void send(const char* message);

    // Returns true if a client is currently connected.
    bool isConnected();

} // namespace Bluetooth
