#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Colors
const uint16_t C_GREEN = 0x07E0;  // Bright green
const uint16_t C_BLACK = TFT_BLACK;
const uint16_t C_YELLOW = 0xFFE0; // Bright yellow

// ── Setup & Loop ─────────────────────────────────────────

void setup() {

    Serial.begin(115200);
    tft.init();
    tft.setRotation(2);  // landscape 320×240  (try 3 if face appears upside-down)
    tft.setAddrWindow(0, 0, 320, 240);  // hardcode your actual dimensions
    tft.pushColor(C_BLACK, 320 * 240);  // flood every pixel

    tft.drawRect(0, 50, 50, 50, C_YELLOW);

}

void loop() {

}
