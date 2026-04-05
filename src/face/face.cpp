#include "face.h"
#include <TFT_eSPI.h>

// ─────────────────────────────────────────────────────────────────────────────
// Display constants
// ─────────────────────────────────────────────────────────────────────────────

static const int SCREEN_W = 320;
static const int SCREEN_H = 240;

// Eye positions (portrait layout: 240 wide, 320 tall — flip if landscape)
static const int EYE_L_X   = 80;
static const int EYE_R_X   = 160;
static const int EYE_Y     = 120;
static const int EYE_R     = 30;

// Mouth position
static const int MOUTH_X   = 120;
static const int MOUTH_Y   = 210;
static const int MOUTH_W   = 80;

// Colors
static const uint16_t C_BG      = TFT_BLACK;
static const uint16_t C_EYE     = TFT_WHITE;
static const uint16_t C_PUPIL   = TFT_BLACK;
static const uint16_t C_MOUTH   = TFT_WHITE;
static const uint16_t C_STATUS  = TFT_YELLOW;

// ─────────────────────────────────────────────────────────────────────────────
// Internal state
// ─────────────────────────────────────────────────────────────────────────────

static TFT_eSPI _tft;
static Face::Emotion _currentEmotion = Face::Emotion::NEUTRAL;
static unsigned long _lastBlinkMs    = 0;
static bool _eyesOpen                = true;

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void Face::init() {
    _tft.init();
    _tft.setRotation(2);
    clear();
    showEmotion(Emotion::NEUTRAL);
}

void Face::clear() {
    _tft.fillScreen(C_BG);
}

void Face::showEmotion(Emotion emotion) {
    _currentEmotion = emotion;
    clear();

    switch (emotion) {
        case Emotion::HAPPY:
            drawEye(EYE_L_X, EYE_Y, EYE_R, true);
            drawEye(EYE_R_X, EYE_Y, EYE_R, true);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W, +1);   // smile
            break;

        case Emotion::SAD:
            drawEye(EYE_L_X, EYE_Y, EYE_R, true);
            drawEye(EYE_R_X, EYE_Y, EYE_R, true);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W, -1);   // frown
            break;

        case Emotion::ANGRY:
            drawEye(EYE_L_X, EYE_Y, EYE_R / 2, true);  // squinted
            drawEye(EYE_R_X, EYE_Y, EYE_R / 2, true);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W, -1);
            break;

        case Emotion::EXCITED:
            // Wide open eyes + big smile
            drawEye(EYE_L_X, EYE_Y, EYE_R + 8, true);
            drawEye(EYE_R_X, EYE_Y, EYE_R + 8, true);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W + 20, +2);
            break;

        case Emotion::SLEEPY:
            drawEye(EYE_L_X, EYE_Y, EYE_R, false);     // closed
            drawEye(EYE_R_X, EYE_Y, EYE_R, false);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W, 0);    // flat
            break;

        case Emotion::NEUTRAL:
        default:
            drawEye(EYE_L_X, EYE_Y, EYE_R, true);
            drawEye(EYE_R_X, EYE_Y, EYE_R, true);
            drawMouth(MOUTH_X, MOUTH_Y, MOUTH_W, 0);
            break;
    }
}

void Face::showStatus(const char* text) {
    // Overwrite the bottom status bar only.
    _tft.fillRect(0, SCREEN_H - 20, SCREEN_W, 20, C_BG);
    _tft.setTextColor(C_STATUS, C_BG);
    _tft.setTextSize(1);
    _tft.setCursor(4, SCREEN_H - 16);
    _tft.print(text);
}

void Face::update() {
    // Simple blink every ~4 s for NEUTRAL / HAPPY / EXCITED.
    if (_currentEmotion == Emotion::SLEEPY) return;

    unsigned long now = millis();
    if (_eyesOpen && (now - _lastBlinkMs > 4000)) {
        // Close eyes briefly
        drawEye(EYE_L_X, EYE_Y, EYE_R, false);
        drawEye(EYE_R_X, EYE_Y, EYE_R, false);
        _eyesOpen    = false;
        _lastBlinkMs = now;
    } else if (!_eyesOpen && (now - _lastBlinkMs > 150)) {
        // Reopen
        showEmotion(_currentEmotion);
        _eyesOpen    = true;
        _lastBlinkMs = now;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Low-level drawing helpers
// ─────────────────────────────────────────────────────────────────────────────

void Face::drawEye(int16_t cx, int16_t cy, int16_t r, bool isOpen) {
    if (isOpen) {
        _tft.fillCircle(cx, cy, r, C_EYE);
        _tft.fillCircle(cx + 4, cy + 4, r / 3, C_PUPIL);  // pupil
    } else {
        // Closed eye: horizontal line
        _tft.drawFastHLine(cx - r, cy, r * 2, C_EYE);
    }
}

void Face::drawMouth(int16_t cx, int16_t cy, int16_t width, int8_t curveDir) {
    int16_t halfW = width / 2;
    if (curveDir == 0) {
        _tft.drawFastHLine(cx - halfW, cy, width, C_MOUTH);
    } else {
        // Approximate curve with three line segments
        int16_t lift = (curveDir > 0) ? -12 : 12;  // smile lifts endpoints up
        _tft.drawLine(cx - halfW, cy,           cx,            cy + lift, C_MOUTH);
        _tft.drawLine(cx,         cy + lift,    cx + halfW,    cy,        C_MOUTH);
    }
}
