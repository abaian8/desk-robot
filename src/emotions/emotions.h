#pragma once
#include "../face/face.h"
#include "../ears/ears.h"

// ─────────────────────────────────────────────────────────────────────────────
// EmotionManager — high-level coordinator that sets the Face AND Ears together
// so the robot expresses a coherent emotion across both subsystems.
//
// This is the only layer that should call Face:: and Ears:: for emotion
// changes.  The rest of the code (Bluetooth command handler, etc.) should
// call EmotionManager::set() instead of touching face/ears directly.
// ─────────────────────────────────────────────────────────────────────────────

namespace EmotionManager {

    // Must be called once in setup() — after Face::init() and Ears::init().
    void init();

    // Apply a named emotion to both face and ears.
    void set(Face::Emotion emotion);

    // Return the currently active emotion.
    Face::Emotion current();

    // Map a string name (from BLE command) to an Emotion enum value.
    // Returns Emotion::NEUTRAL if the name is unrecognised.
    Face::Emotion fromString(const char* name);

} // namespace EmotionManager
