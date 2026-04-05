#pragma once
#include <stdint.h>

// ─────────────────────────────────────────────────────────────────────────────
// FaceModule — drives the 2.8" SPI TFT LCD (320×240) via TFT_eSPI.
//
// Provides:
//   - Emotion face rendering (eyes + mouth shapes)
//   - Status / text overlay display
//   - Raw drawing access for custom visuals
//
// All draw calls are blocking.  For animations, call update() from loop().
// ─────────────────────────────────────────────────────────────────────────────

namespace Face {

    enum class Emotion { NEUTRAL, HAPPY, SAD, ANGRY, EXCITED, SLEEPY };

    // Must be called once in setup().
    void init();

    // Render a full-face emotion expression.
    void showEmotion(Emotion emotion);

    // Show a short text status line at the bottom of the display.
    void showStatus(const char* text);

    // Clear the screen to black.
    void clear();

    // Call from loop() to handle any animation ticks (blinking, etc.).
    void update();

    // ── Low-level drawing helpers (used internally, exposed for extensibility) ──

    // Draw a single eye at (cx, cy) with radius r.  isOpen controls open/closed.
    void drawEye(int16_t cx, int16_t cy, int16_t r, bool isOpen);

    // Draw a mouth curve.  curveDir > 0 = smile, < 0 = frown, 0 = flat.
    void drawMouth(int16_t cx, int16_t cy, int16_t width, int8_t curveDir);

} // namespace Face
