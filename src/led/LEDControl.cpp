#include "LEDControl.h"
#include "Pins.h"

#include <Adafruit_NeoPixel.h>

#include <map>

#define DEFAULT_PROGRAM 75

#define KICK_COUNT 27
#define SNARE_COUNT 45
#define TOM_COUNT 33
#define CRASH_COUNT 17

#define LED_COUNT KICK_COUNT + SNARE_COUNT + TOM_COUNT * 3 + CRASH_COUNT * 2

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

std::vector<Drum> order = {Drum::kick, Drum::tom3, Drum::crash2, Drum::tom2, Drum::tom1, Drum::crash1, Drum::snare};
std::map<Drum, uint8_t> startIndex;

byte program = DEFAULT_PROGRAM;
byte hihatState = 0;

uint16_t color = 65535 / 360 * 240;

void LEDControl::setup() {
    initializeDrums();

    pinMode(LED_PIN, OUTPUT);
    leds.begin();
    leds.fill(Adafruit_NeoPixel::ColorHSV(color, 255, 255), 0, LED_COUNT);
    leds.show();
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
    color = color + 1;
    leds.fill(Adafruit_NeoPixel::ColorHSV(color, 255, 255), 0, LED_COUNT);
    leds.show();
}

void LEDControl::handleNoteOn(NoteOn note) {
    Drum drumNote = Note::getDrum(note.note);
    // TODO: leds.fill(Adafruit_NeoPixel::Color(255, 255, 255), startIndex[drumNote], getCountFor(drumNote));
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

uint8_t LEDControl::getCountFor(Drum drum) {
    switch (drum) {
        case Drum::kick:
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

void LEDControl::initializeDrums() {
    uint8_t currentIndex = 0;
    for (Drum current: order) {
        startIndex.insert({current, currentIndex});
        currentIndex += getCountFor(current);
    }
}
