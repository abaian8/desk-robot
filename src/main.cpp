#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Colors
const uint16_t C_GREEN = 0x07E0;  // Bright green
const uint16_t C_BLACK = TFT_BLACK;

// Eye geometry (pixels)
const int EYE_W     = 80;
const int EYE_H     = 58;
const int EYE_BOR   = 4;    // border thickness
const int PUPIL_W   = 22;
const int PUPIL_H   = 30;
const int PUPIL_MAX = 16;   // max pupil travel offset

// Face layout — landscape 320x240
const int L_EYE_X  = 88;
const int R_EYE_X  = 232;
const int EYE_CY   = 90;

const int MOUTH_CX = 160;
const int MOUTH_Y  = 172;
const int M_BS     = 8;     // mouth block size

// ── Drawing ──────────────────────────────────────────────

void drawEye(int cx, int cy, int offX, int offY, bool closed) {
    // Clear eye zone
    tft.fillRect(cx - EYE_W/2 - 1, cy - EYE_H/2 - 1, EYE_W + 2, EYE_H + 2, C_BLACK);

    if (closed) {
        // Closed / wink: single horizontal bar
        tft.fillRect(cx - EYE_W/2, cy - 3, EYE_W, 6, C_GREEN);
        return;
    }

    // Rectangular outline
    tft.fillRect(cx - EYE_W/2,           cy - EYE_H/2,           EYE_W,   EYE_BOR, C_GREEN); // top
    tft.fillRect(cx - EYE_W/2,           cy + EYE_H/2 - EYE_BOR, EYE_W,   EYE_BOR, C_GREEN); // bottom
    tft.fillRect(cx - EYE_W/2,           cy - EYE_H/2,           EYE_BOR, EYE_H,   C_GREEN); // left
    tft.fillRect(cx + EYE_W/2 - EYE_BOR, cy - EYE_H/2,           EYE_BOR, EYE_H,   C_GREEN); // right

    // Pupil — clamped inside eye interior
    int px = constrain(cx + offX - PUPIL_W/2,
                       cx - EYE_W/2 + EYE_BOR + 2,
                       cx + EYE_W/2 - EYE_BOR - PUPIL_W - 2);
    int py = constrain(cy + offY - PUPIL_H/2,
                       cy - EYE_H/2 + EYE_BOR + 2,
                       cy + EYE_H/2 - EYE_BOR - PUPIL_H - 2);

    tft.fillRect(px, py, PUPIL_W, PUPIL_H, C_GREEN);
}

void drawMouth() {
    int cx = MOUTH_CX;
    int y  = MOUTH_Y;
    int bs = M_BS;

    tft.fillRect(cx - 48, y - 1, 96, bs * 3 + 2, C_BLACK);

    // Pixel-art smile — 3-row U shape, each block = 8×8 px
    //
    //  ##              ##    ← row 0 (outer corners, highest)
    //    ##          ##      ← row 1 (inner curve)
    //        ########        ← row 2 (bottom centre)

    tft.fillRect(cx - 48, y,        bs * 2, bs, C_GREEN); // left  outer
    tft.fillRect(cx + 32, y,        bs * 2, bs, C_GREEN); // right outer

    tft.fillRect(cx - 32, y + bs,   bs * 2, bs, C_GREEN); // left  curve
    tft.fillRect(cx + 16, y + bs,   bs * 2, bs, C_GREEN); // right curve

    tft.fillRect(cx - 16, y + bs*2, bs * 4, bs, C_GREEN); // centre bottom
}

void drawFace(int px, int py, bool rClosed = false, bool lClosed = false) {
    drawEye(L_EYE_X, EYE_CY, px, py, lClosed);
    drawEye(R_EYE_X, EYE_CY, px, py, rClosed);
}

// Smooth interpolated pupil movement
void movePupilTo(int fromX, int fromY, int toX, int toY,
                 int steps, int stepDelay,
                 bool rClosed = false, bool lClosed = false) {
    for (int i = 1; i <= steps; i++) {
        int cx = fromX + (toX - fromX) * i / steps;
        int cy = fromY + (toY - fromY) * i / steps;
        drawFace(cx, cy, rClosed, lClosed);
        delay(stepDelay);
    }
}

void blink() {
    drawFace(0, 0, true, true);
    delay(120);
    drawFace(0, 0, false, false);
}

// ── Setup & Loop ─────────────────────────────────────────

void setup() {

    tft.init();
    tft.setRotation(2);   // landscape 320×240  (try 3 if face appears upside-down)
    tft.fillScreen(C_BLACK);

    drawMouth();
    drawFace(0, 0);
    delay(1000);
}

void loop() {

    // Hold center gaze
    delay(800);

    // Look left
    movePupilTo(0, 0, -PUPIL_MAX, 0, 8, 35);
    delay(600);

    // Look right
    movePupilTo(-PUPIL_MAX, 0, PUPIL_MAX, 0, 12, 35);
    delay(600);

    // Return to center
    movePupilTo(PUPIL_MAX, 0, 0, 0, 8, 35);
    delay(400);

    // Wink right eye (double flick)
    drawFace(0, 0, true, false);
    delay(160);
    drawFace(0, 0, false, false);
    delay(100);
    drawFace(0, 0, true, false);
    delay(160);
    drawFace(0, 0, false, false);
    delay(600);

    // Idle micro-drift (subtle eye wander)
    movePupilTo( 0,  0,  4,  2, 6, 50);
    delay(350);
    movePupilTo( 4,  2, -4, -2, 8, 50);
    delay(350);
    movePupilTo(-4, -2,  0,  0, 6, 50);
    delay(400);

    // Idle blink
    blink();
    delay(200);
}
