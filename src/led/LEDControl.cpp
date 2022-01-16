#include "LEDControl.h"

#include <NeoPixelAnimator.h>

#include <map>

#define DEFAULT_PROGRAM 75

#define KICK_COUNT 27
#define SNARE_COUNT 45
#define TOM_COUNT 33
#define CRASH_COUNT 17

#define LED_COUNT KICK_COUNT + SNARE_COUNT + TOM_COUNT * 3 + CRASH_COUNT * 2

std::vector<Drum> order = {Drum::kick, Drum::tom3, Drum::crash2, Drum::tom2, Drum::tom1, Drum::crash1, Drum::snare};
std::map<Drum, uint8_t> startIndex;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(LED_COUNT);
NeoPixelAnimator animations(order.size() + 1);

RgbColor white(255);
RgbColor black(0);

byte program = DEFAULT_PROGRAM;
byte hihatState = 0;

float currentHue = 240;

void LEDControl::setup() {
    initializeDrums();

    leds.Begin();
    leds.ClearTo(HslColor(currentHue / 360.0f, 1.0, 0.5));
    leds.Show();
}

void LEDControl::handleMidi(std::variant<NoteOn, NoteOff, Pressure, ControlChange, ProgramChange> midiMsg) {
    std::visit([](auto &&msg) {
        using T = std::decay_t<decltype(msg)>;
        if constexpr (std::is_same_v<T, NoteOn>) {
            handleNoteOn(msg);
        } else if constexpr (std::is_same_v<T, NoteOff>) {
            handleNoteOff(msg);
        } else if constexpr (std::is_same_v<T, Pressure>) {
            handlePressure(msg);
        } else if constexpr (std::is_same_v<T, ControlChange>) {
            handleControlChange(msg);
        } else if constexpr (std::is_same_v<T, ProgramChange>) {
            handleProgramChange(msg);
        }
    }, midiMsg);
}

void LEDControl::update() {
    if (leds.CanShow()) {
        rotateHue(0.1);
        for (Drum drum: order) {
            if (!animations.IsAnimationActive(drum)) {
                setDrumColor(getBaseColorFor(drum), drum);
            }
        }
        animations.UpdateAnimations();
        leds.Show();
    }
}

void LEDControl::handleNoteOn(NoteOn note) {
    Drum drumNote = Note::getDrum(note.note);
    if (std::find(order.begin(), order.end(), drumNote) != order.end()) {
        AnimUpdateCallback animUpdate = [=](const AnimationParam &param) {
            float progress = NeoEase::CubicOut(param.progress);
            RgbColor updatedColor = RgbColor::LinearBlend(white, getBaseColorFor(drumNote), progress);
            setDrumColor(updatedColor, drumNote);
        };
        animations.StartAnimation(drumNote, note.velocity * 15, animUpdate);
    } else if (drumNote == hihat && hihatState < 45) {
        rotateHue(40);
    }
}

void LEDControl::handleNoteOff(__attribute__((unused)) NoteOff note) {
    // ignore note off, not relevant
}

void LEDControl::handlePressure(__attribute__((unused)) Pressure note) {
    // unused for now
}

void LEDControl::handleControlChange(ControlChange controlChange) {
    if (controlChange.control == HIHAT_CONTROL) {
        hihatState = controlChange.value;
    }
}

void LEDControl::handleProgramChange(ProgramChange programChange) {
    program = programChange.program;
}

void LEDControl::initializeDrums() {
    uint8_t currentIndex = 0;
    for (Drum current: order) {
        startIndex.insert({current, currentIndex});
        currentIndex += getCountFor(current);
    }
}

uint8_t LEDControl::getCountFor(Drum drum) {
    switch (drum) {
        case kick:
            return KICK_COUNT;
        case snare:
            return SNARE_COUNT;
        case tom1:
        case tom2:
        case tom3:
            return TOM_COUNT;
        case crash1:
        case crash2:
            return CRASH_COUNT;
        default:
            return 0;
    }
}

NeoGrbFeature::ColorObject LEDControl::getBaseColorFor(Drum drum) {
    switch (drum) {
        case kick:
        case snare:
        case tom1:
        case tom2:
        case tom3:
            return HslColor(currentHue / 360.0f, 1.0, 0.5);
        case crash1:
        case crash2:
        default:
            return black;
    }
}

void LEDControl::setDrumColor(NeoGrbFeature::ColorObject color, Drum drum) {
    leds.ClearTo(color, startIndex[drum], startIndex[drum] + getCountFor(drum) - 1);
}

void LEDControl::rotateHue(float steps) {
    currentHue += steps;
    while (currentHue > 360) currentHue -= 360;
}
