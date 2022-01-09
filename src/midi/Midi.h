#ifndef DRUM_MIDI_LED_CONTROL_MIDI_H
#define DRUM_MIDI_LED_CONTROL_MIDI_H

#include <Arduino.h>

class Midi {
public:
    static void setup();
    static void read();

private:
    static void printMidiCommand(String name, byte channel, byte data1, byte data2);
    static void receiveNoteOn(byte note, byte velocity);
    static void receiveNoteOff(byte note, byte velocity);
    static void receivePressure(byte note, byte value);
    static void receiveControlChange(byte control, byte value);
    static void receiveProgramChange(byte program);
};

#endif //DRUM_MIDI_LED_CONTROL_MIDI_H
