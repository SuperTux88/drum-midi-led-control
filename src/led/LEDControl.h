#ifndef DRUM_MIDI_LED_CONTROL_LEDCONTROL_H
#define DRUM_MIDI_LED_CONTROL_LEDCONTROL_H

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

    static uint8_t getCountFor(Drum drum);
    static void initializeDrums();
};

#endif //DRUM_MIDI_LED_CONTROL_LEDCONTROL_H
