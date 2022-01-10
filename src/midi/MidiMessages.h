#ifndef DRUM_MIDI_LED_CONTROL_MIDIMESSAGES_H
#define DRUM_MIDI_LED_CONTROL_MIDIMESSAGES_H

#include <Arduino.h>

struct MidiMessage {
    const byte channel;

protected:
    explicit MidiMessage(byte channel);
};

struct NoteOn : MidiMessage {
    const byte note;
    const byte velocity;

    NoteOn(byte channel, byte note, byte velocity);
};

struct NoteOff : MidiMessage {
    const byte note;
    const byte velocity;

    NoteOff(byte channel, byte note, byte velocity);
};

struct Pressure : MidiMessage {
    const byte note;
    const byte value;

    Pressure(byte channel, byte note, byte value);
};

struct ControlChange : MidiMessage {
    const byte control;
    const byte value;

    ControlChange(byte channel, byte control, byte value);
};

struct ProgramChange : MidiMessage {
    const byte program;

    ProgramChange(byte channel, byte program);
};

#endif //DRUM_MIDI_LED_CONTROL_MIDIMESSAGES_H
