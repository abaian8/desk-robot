#include "emotions.h"
#include <string.h>

// ─────────────────────────────────────────────────────────────────────────────
// Emotion → ear position mapping table
// ─────────────────────────────────────────────────────────────────────────────

struct EmotionProfile {
    Face::Emotion  emotion;
    Ears::Position ears;
};

static const EmotionProfile _profiles[] = {
    { Face::Emotion::NEUTRAL,  Ears::Position::NEUTRAL },
    { Face::Emotion::HAPPY,    Ears::Position::UP      },
    { Face::Emotion::EXCITED,  Ears::Position::UP      },
    { Face::Emotion::SAD,      Ears::Position::DOWN    },
    { Face::Emotion::SLEEPY,   Ears::Position::DOWN    },
    { Face::Emotion::ANGRY,    Ears::Position::NEUTRAL },
};

static Face::Emotion _currentEmotion = Face::Emotion::NEUTRAL;

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

static Ears::Position _earsForEmotion(Face::Emotion e) {
    for (auto& p : _profiles)
        if (p.emotion == e) return p.ears;
    return Ears::Position::NEUTRAL;
}

// ─────────────────────────────────────────────────────────────────────────────
// Public API
// ─────────────────────────────────────────────────────────────────────────────

void EmotionManager::init() {
    set(Face::Emotion::NEUTRAL);
}

void EmotionManager::set(Face::Emotion emotion) {
    _currentEmotion = emotion;
    Face::showEmotion(emotion);
    Ears::setPosition(_earsForEmotion(emotion));
}

Face::Emotion EmotionManager::current() {
    return _currentEmotion;
}

Face::Emotion EmotionManager::fromString(const char* name) {
    if (strcasecmp(name, "HAPPY")   == 0) return Face::Emotion::HAPPY;
    if (strcasecmp(name, "SAD")     == 0) return Face::Emotion::SAD;
    if (strcasecmp(name, "ANGRY")   == 0) return Face::Emotion::ANGRY;
    if (strcasecmp(name, "EXCITED") == 0) return Face::Emotion::EXCITED;
    if (strcasecmp(name, "SLEEPY")  == 0) return Face::Emotion::SLEEPY;
    return Face::Emotion::NEUTRAL;
}
