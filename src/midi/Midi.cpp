#include "Midi.h"
#include "Drum.h"
#include "Pins.h"

#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial midiSerial(MIDI_PIN);

void Midi::setup() {
    pinMode(MIDI_PIN, INPUT);
    midiSerial.begin(31250);

    pinMode(DEBUG_PIN, OUTPUT);
}

byte command;
byte channel;
byte data[2];
byte dataIndex = 0;

std::optional<std::variant<NoteOn, NoteOff, Pressure, ControlChange, ProgramChange>> Midi::read() {
    while (midiSerial.available() > 0) {
        byte message = midiSerial.read();
        if (message > 127) { // new command
            command = message >> 4;
            channel = (message & 15) + 1;
            dataIndex = 0;
        } else {
            data[dataIndex++] = message;
        }
        if (command == 8) {
            if (dataIndex == 2) {
                dataIndex = 0;
                return receiveNoteOff(data[0], data[1]);
            }
        } else if (command == 9) {
            if (dataIndex == 2) {
                dataIndex = 0;
                return receiveNoteOn(data[0], data[1]);
            }
        } else if (command == 10) {
            if (dataIndex == 2) {
                dataIndex = 0;
                return receivePressure(data[0], data[1]);
            }
        } else if (command == 11) {
            if (dataIndex == 2) {
                dataIndex = 0;
                return receiveControlChange(data[0], data[1]);
            }
        } else if (command == 12) {
            if (dataIndex == 1) {
                dataIndex = 0;
                return receiveProgramChange(data[0] + 1);
            }
        } else if (message != 254 && // ignore Active Sensing messages
                   message != 248) { // ignore Timing Clock messages
            dataIndex = 0;
            Serial.printf("Unknown/Unexpected Message: %d\n", message);
        }
    }
    return std::nullopt;
}

NoteOn Midi::receiveNoteOn(byte note, byte velocity) {
#if DEBUG
    Serial.printf("Note On:\tChannel: %3d Note: %-18s Velocity: %3d\n", channel, Note::getText(note).c_str(), velocity);
    digitalWrite(DEBUG_PIN, LOW);
#endif
    return {channel, note, velocity};
}

NoteOff Midi::receiveNoteOff(byte note, byte velocity) {
#if DEBUG
    Serial.printf("Note Off:\tChannel: %3d Note: %-18s Velocity: %3d\n", channel, Note::getText(note).c_str(), velocity);
    digitalWrite(DEBUG_PIN, HIGH);
#endif
    return {channel, note, velocity};
}

Pressure Midi::receivePressure(byte note, byte value) {
#if DEBUG
    Serial.printf("Pressure:\tChannel: %3d Note: %-18s Value: %3d\n", channel, Note::getText(note).c_str(), value);
#endif
    return {channel, note, value};
}

ControlChange Midi::receiveControlChange(byte control, byte value) {
#if DEBUG
    Serial.printf("Control Change:\tChannel: %3d Control: %3d Value: %3d\n", channel, control, value);
#endif
    return {channel, control, value};
}

ProgramChange Midi::receiveProgramChange(byte program) {
#if DEBUG
    Serial.printf("Program Change:\tChannel: %3d Program: %3d\n", channel, program);
#endif
    return {channel, program};
}
