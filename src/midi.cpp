#include "midi.h"

#define MIDI_PIN 4

#define DEBUG true
#define DEBUG_PIN 2 // onboard LED

SoftwareSerial midiSerial(MIDI_PIN);


void setupMidi() {
    pinMode(MIDI_PIN, INPUT);
    midiSerial.begin(31250);

    pinMode(DEBUG_PIN, OUTPUT);
}

byte command;
byte channel;
byte data[2];
byte dataIndex = 0;

void readMidi() {
    if (midiSerial.available() > 0) {
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
                receiveNoteOff(data[0], data[1]);
                dataIndex = 0;
            }
        } else if (command == 9) {
            if (dataIndex == 2) {
                receiveNoteOn(data[0], data[1]);
                dataIndex = 0;
            }
        } else if (command == 10) {
            if (dataIndex == 2) {
                receivePressure(data[0], data[1]);
                dataIndex = 0;
            }
        } else if (command == 11) {
            if (dataIndex == 2) {
                receiveControlChange(data[0], data[1]);
                dataIndex = 0;
            }
        } else if (command == 12) {
            if (dataIndex == 1) {
                receiveProgramChange(data[0] + 1);
                dataIndex = 0;
            }
        } else if (message != 254 && // ignore Active Sensing messages
                   message != 248) { // ignore Timing Clock messages
            if (dataIndex == 0) {
                printMidiCommand("Unknown", message, 0, 0);
                dataIndex = 0;
            }
        }
    }
}

void printMidiCommand(String name, byte channel, byte data1, byte data2) {
    Serial.print(name);
    Serial.print(":\t");
    Serial.print(channel);
    if (dataIndex >= 1) {
        Serial.print("\t");
        Serial.print(data1);
    }
    if (dataIndex >= 2) {
        Serial.print("\t");
        Serial.print(data2);
    }
    Serial.println();
}

void receiveNoteOn(byte note, byte velocity) {
#if DEBUG
    printMidiCommand("Note On", channel, note, velocity);
    digitalWrite(DEBUG_PIN, LOW);
#endif

}

void receiveNoteOff(byte note, byte velocity) {
#if DEBUG
    printMidiCommand("Note Off", channel, note, velocity);
    digitalWrite(DEBUG_PIN, HIGH);
#endif

}

void receivePressure(byte note, byte value) {
#if DEBUG
    printMidiCommand("Pressure", channel, note, value);
#endif

}

void receiveControlChange(byte control, byte value) {
#if DEBUG
    printMidiCommand("Control", channel, control, value);
#endif

}

void receiveProgramChange(byte program) {
#if DEBUG
    printMidiCommand("Program", channel, program, 0);
#endif

}
