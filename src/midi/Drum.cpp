#include "Drum.h"

#include <sstream>

Drum Note::getDrum(byte note) {
    switch (note) {
        case KICK:
            return Drum::kick;
        case SNARE:
        case SNARE_XSTICK:
        case SNARE_RIM:
            return Drum::snare;
        case TOM1:
        case TOM1_RIM:
            return Drum::tom1;
        case TOM2:
        case TOM2_RIM:
            return Drum::tom2;
        case TOM3:
        case TOM3_RIM:
            return Drum::tom3;
        case HIHAT:
        case HIHAT_EDGE:
        case HIHAT_FOOT:
            return Drum::hihat;
        case RIDE:
        case RIDE_EDGE:
        case RIDE_BELL:
            return Drum::ride;
        case CRASH1:
        case CRASH1_EDGE:
            return Drum::crash1;
        case CRASH2:
        case CRASH2_EDGE:
            return Drum::crash2;
        default:
            return Drum::unknown;
    }
}

std::string Note::getDrumText(byte note) {
    switch (note) {
        case KICK:         return "Kick";
        case SNARE:        return "Snare";
        case SNARE_XSTICK: return "Snare Cross-Stick";
        case SNARE_RIM:    return "Snare Rim";
        case TOM1:         return "Tom 1";
        case TOM1_RIM:     return "Tom 1 Rim";
        case TOM2:         return "Tom 2";
        case TOM2_RIM:     return "Tom 2 Rim";
        case TOM3:         return "Tom 3";
        case TOM3_RIM:     return "Tom 3 Rim";
        case HIHAT:        return "Hi-Hat";
        case HIHAT_EDGE:   return "Hi-Hat Edge";
        case HIHAT_FOOT:   return "Hi-Hat Foot";
        case RIDE:         return "Ride";
        case RIDE_EDGE:    return "Ride Edge";
        case RIDE_BELL:    return "Ride Bell";
        case CRASH1:       return "Crash 1";
        case CRASH1_EDGE:  return "Crash 1 Edge";
        case CRASH2:       return "Crash 2";
        case CRASH2_EDGE:  return "Crash 2 Edge";
        default:
            std::ostringstream unknown;
            unknown << "Unknown: " << std::to_string((unsigned) note);
            return unknown.str();
    }
}

Drum Note::getControl(byte control) {
    switch (control) {
        case SNARE_CONTROL: return Drum::snare;
        case HIHAT_CONTROL: return Drum::hihat;
        case RIDE_CONTROL:  return Drum::ride;
        default:            return Drum::unknown;
    }
}

std::string Note::getControlText(byte control) {
    switch (control) {
        case SNARE_CONTROL: return "Snare Control";
        case HIHAT_CONTROL: return "Hi-Hat Control";
        case RIDE_CONTROL:  return "Ride Control";
        default:
            std::ostringstream unknown;
            unknown << "Unknown: " << std::to_string((unsigned) control);
            return unknown.str();
    }
}
