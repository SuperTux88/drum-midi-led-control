#ifndef DRUM_MIDI_LED_CONTROL_LEDCONTROL_H
#define DRUM_MIDI_LED_CONTROL_LEDCONTROL_H

#include <NeoPixelBus.h>

#include <variant>

#include "midi/Drum.h"
#include "midi/MidiMessages.h"

class LEDControl {
public:
    static void setup();
    static void handleMidi(std::variant<NoteOn, NoteOff, Pressure, ControlChange, ProgramChange> midiMsg);
    static void update();

private:
    static void handleNoteOn(NoteOn note);
    static void handleNoteOff(NoteOff note);
    static void handlePressure(Pressure note);
    static void handleControlChange(ControlChange controlChange);
    static void handleProgramChange(ProgramChange programChange);

    static void initializeDrums();
    static uint8_t getCountFor(Drum drum);
    static uint8_t getInsertOffsetFor(Drum drum);
    static NeoGrbFeature::ColorObject getBaseColorFor(Drum drum);
    static void setDrumColor(Drum drum, NeoGrbFeature::ColorObject color);
    static void rotateLeft(Drum drum, uint16_t count = 1);
    static void rotateRight(Drum drum, uint16_t count = 1);

    static void rotateHue(float steps);
};

#endif //DRUM_MIDI_LED_CONTROL_LEDCONTROL_H
