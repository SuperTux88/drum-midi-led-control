#ifndef DRUM_MIDI_LED_CONTROL_DRUM_H
#define DRUM_MIDI_LED_CONTROL_DRUM_H

#include <string>

#include <Arduino.h>

#define KICK 36

#define SNARE 38
#define SNARE_XSTICK 37
#define SNARE_RIM 40
#define SNARE_CONTROL 16

#define TOM1 48
#define TOM1_RIM 50
#define TOM2 45
#define TOM2_RIM 47
#define TOM3 43
#define TOM3_RIM 58

#define HIHAT 46
#define HIHAT_EDGE 26
#define HIHAT_FOOT 44
#define HIHAT_CONTROL 4

#define RIDE 51
#define RIDE_EDGE 59
#define RIDE_BELL 53
#define RIDE_CONTROL 17

#define CRASH1 49
#define CRASH1_EDGE 55
#define CRASH2 57
#define CRASH2_EDGE 52

enum Drum {
    kick, snare, tom1, tom2, tom3, crash1, crash2, hihat, ride, unknown
};

class Note {
public:
    static Drum getDrum(byte note);
    static std::string getDrumText(byte note);
    static Drum getControl(byte note);
    static std::string getControlText(byte note);
};

#endif //DRUM_MIDI_LED_CONTROL_DRUM_H
