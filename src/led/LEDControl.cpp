#include "LEDControl.h"
#include "Config.h"

#include <NeoPixelAnimator.h>

#include <map>

#define DEFAULT_PROGRAM 75

#define KICK_COUNT 27
#define SNARE_COUNT 45
#define TOM_COUNT 33
#define CRASH_COUNT 17

#define LED_COUNT KICK_COUNT + SNARE_COUNT + TOM_COUNT * 3 + CRASH_COUNT * 2

std::vector<Drum> order = {kick, tom3, crash2, tom2, tom1, crash1, snare};
std::map<Drum, uint8_t> startIndex;

std::map<Drum, unsigned long> lastHit;
std::map<Drum, unsigned long> lastOff;
std::map<Drum, byte> lastNote;
std::map<Drum, byte> lastVelocity;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(LED_COUNT);
NeoPixelAnimator animations(order.size() + 1);
NeoGamma<NeoGammaTableMethod> colorGamma;

RgbColor white(255);
RgbColor black(0);

byte program = DEFAULT_PROGRAM;
byte hihatState = 0;

float currentHue = 120;

void LEDControl::setup() {
    leds.Begin();
    leds.ClearTo(black);
    leds.Show();

    initializeDrums();
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
                setDrumColor(drum, getBaseColorFor(drum));
            }
        }
        animations.UpdateAnimations();
        leds.Show();
    }
}

void LEDControl::handleNoteOn(NoteOn note) {
    Drum drumNote = Note::getDrum(note.note);
    if (std::find(order.begin(), order.end(), drumNote) != order.end()) {
        if (handleDebounce(note)) return;

        lastHit[drumNote] = millis();
        lastNote[drumNote] = note.note;
        lastVelocity[drumNote] = note.velocity;

        AnimUpdateCallback animUpdate = [=](const AnimationParam &param) {
            float progress = NeoEase::CubicOut(param.progress);
            setDrumColor(drumNote,
                         RgbColor::LinearBlend(getHighlightColorFor(drumNote), getBaseColorFor(drumNote), progress));
        };
        animations.StartAnimation(drumNote, note.velocity * getAnimationMultiplierFor(drumNote), animUpdate);
    } else if (drumNote == hihat && hihatState < 75) {
        rotateHue(40);
    }
}

void LEDControl::handleNoteOff(NoteOff note) {
    Drum drumNote = Note::getDrum(note.note);
    lastOff[drumNote] = millis();
}

void LEDControl::handlePressure(__attribute__((unused)) Pressure note) {
    // unused for now
}

void LEDControl::handleControlChange(ControlChange controlChange) {
    Drum drumNote = Note::getControl(controlChange.control);
    if (drumNote == hihat) {
        hihatState = controlChange.value;
    }
}

void LEDControl::handleProgramChange(ProgramChange programChange) {
    program = programChange.program;
}

void LEDControl::initializeDrums() {
    uint8_t currentIndex = 0;
    for (Drum currentDrum: order) {
        startIndex[currentDrum] = currentIndex;
        lastHit[currentDrum] = 0;
        lastNote[currentDrum] = 0;
        lastVelocity[currentDrum] = 0;

        currentIndex += getCountFor(currentDrum);

        playStartAnimation(currentDrum);
    }
}

boolean LEDControl::handleDebounce(NoteOn note) {
    Drum drumNote = Note::getDrum(note.note);
    // handle double kick trigger
    if (drumNote == kick && millis() - lastOff[kick] < 50 && note.velocity <= 25 &&
        note.velocity <= lastVelocity[kick] / 2) {

#if DEBUG
        Serial.printf("Ignore double kick for %d/%d: %lums/%lums\n", lastVelocity[drumNote], note.velocity,
                      millis() - lastHit[drumNote], millis() - lastOff[drumNote]);
#endif
        return true; // ignore ghost double trigger
    }
    // sometimes there is a second crash trigger on a different note
    else if (millis() - lastHit[drumNote] < 500 && lastNote[drumNote] != note.note &&
             note.velocity < 15 && lastVelocity[drumNote] - note.velocity > 10) {
#if DEBUG
        Serial.printf("Ignore ghost trigger for %s/%d: %lums\n", Note::getDrumText(note.note).c_str(),
                      note.velocity, millis() - lastHit[drumNote]);
#endif
        return true; // ignore ghost double trigger
    }


    return false; // no debounce
}

void LEDControl::playStartAnimation(Drum drum) {
    // only play start animation for drums with color :)
    if (getBaseColorFor(drum) == black) return;

    uint8_t count = getCountFor(drum);
    AnimUpdateCallback animUpdate = [=](const AnimationParam &param) {
        uint8_t animationCount = count;
        if (param.progress < 0.5) {
            animationCount = (uint8_t) (param.progress * 2 * (float) count);
            setDrumColor(drum, black);
        }
        for (uint8_t i = 0; i < animationCount; i++) {
            auto pixelColor = HslColor(360.0f / (float) count * (float) i / 360.0f, 1.0, 0.5);
            if (param.progress > 0.5) {
                float progress = (param.progress - 0.5f) * 2;
                pixelColor = HslColor::LinearBlend<NeoHueBlendCounterClockwiseDirection>(
                        pixelColor, getBaseColorFor(drum), progress);
            }
            setDrumPixelColor(drum, (int16_t) (-1 - i), pixelColor);
        }
        rotateLeft(drum, getInsertOffsetFor(drum));
    };
    animations.StartAnimation(drum, 3000, animUpdate);
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

uint8_t LEDControl::getInsertOffsetFor(Drum drum) {
    switch (drum) {
        case snare:
            return SNARE_COUNT / 2;
        default:
            return 0;
    }
}

NeoGrbFeature::ColorObject LEDControl::getBaseColorFor(Drum drum) {
    switch (drum) {
        case kick:
        case snare:
            return HslColor(currentHue / 360.0f, 1.0, 0.35);
        case tom1:
        case tom2:
        case tom3:
            return HslColor(currentHue / 360.0f, 1.0, 0.2);
        default:
            return black;
    }
}

NeoGrbFeature::ColorObject LEDControl::getHighlightColorFor(Drum drum) {
    switch (drum) {
        case tom1:
        case tom2:
        case tom3:
            return HslColor(currentHue / 360.0f, 1.0, 0.8);
        default:
            return white;
    }
}

uint8_t LEDControl::getAnimationMultiplierFor(Drum drum) {
    switch (drum) {
        case kick:
        case snare:
        case tom1:
        case tom2:
        case tom3:
            return 10;
        case crash1:
        case crash2:
            return 20;
        default:
            return 0;
    }
}

void LEDControl::setDrumColor(Drum drum, NeoGrbFeature::ColorObject color) {
    leds.ClearTo(colorGamma.Correct(color), startIndex[drum], startIndex[drum] + getCountFor(drum) - 1);
}

void LEDControl::setDrumPixelColor(Drum drum, int16_t index, NeoGrbFeature::ColorObject color) {
    uint16_t indexPixel = (unsigned) startIndex[drum] + index;
    if (index < 0) {
        indexPixel += getCountFor(drum);
    }
    leds.SetPixelColor(indexPixel, colorGamma.Correct(color));
}

void LEDControl::rotateLeft(Drum drum, uint16_t count) {
    leds.RotateLeft(count, startIndex[drum], startIndex[drum] + getCountFor(drum) - 1);
}

void LEDControl::rotateRight(Drum drum, uint16_t count) {
    leds.RotateRight(count, startIndex[drum], startIndex[drum] + getCountFor(drum) - 1);
}

void LEDControl::rotateHue(float steps) {
    currentHue += steps;
    while (currentHue > 360) currentHue -= 360;
}
