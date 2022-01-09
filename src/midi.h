#include "Arduino.h"

#include <SoftwareSerial.h>

#ifndef DRUM_MIDI_LED_CONTROL_MIDI_H
#define DRUM_MIDI_LED_CONTROL_MIDI_H

void setupMidi();
void readMidi();

void printMidiCommand(String name, byte channel, byte data1, byte data2);
void receiveNoteOn(byte note, byte velocity);
void receiveNoteOff(byte note, byte velocity);
void receivePressure(byte note, byte value);
void receiveControlChange(byte control, byte value);
void receiveProgramChange(byte program);

#endif //DRUM_MIDI_LED_CONTROL_MIDI_H
