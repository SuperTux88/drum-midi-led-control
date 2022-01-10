#ifndef DRUM_MIDI_LED_CONTROL_MIDI_H
#define DRUM_MIDI_LED_CONTROL_MIDI_H

#include <Arduino.h>

#include <optional>
#include <variant>

#include "MidiMessages.h"

class Midi {
public:
    static void setup();
    static std::optional<std::variant<NoteOn, NoteOff, Pressure, ControlChange, ProgramChange>> read();

private:
    static NoteOn receiveNoteOn(byte note, byte velocity);
    static NoteOff receiveNoteOff(byte note, byte velocity);
    static Pressure receivePressure(byte note, byte value);
    static ControlChange receiveControlChange(byte control, byte value);
    static ProgramChange receiveProgramChange(byte program);
};

#endif //DRUM_MIDI_LED_CONTROL_MIDI_H
