#include "MidiMessages.h"

MidiMessage::MidiMessage(byte channel) : channel(channel) {}

NoteOn::NoteOn(byte channel, byte note, byte velocity) :
        MidiMessage(channel), note(note), velocity(velocity) {}

NoteOff::NoteOff(byte channel, byte note, byte velocity) :
        MidiMessage(channel), note(note), velocity(velocity) {}

Pressure::Pressure(byte channel, byte note, byte value) :
        MidiMessage(channel), note(note), value(value) {}

ControlChange::ControlChange(byte channel, byte control, byte value) :
        MidiMessage(channel), control(control), value(value) {}

ProgramChange::ProgramChange(byte channel, byte program) :
        MidiMessage(channel), program(program) {}
