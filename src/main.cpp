#include <Arduino.h>

#include "midi/Midi.h"
#include "Pins.h"

[[maybe_unused]]
void setup() {
    Midi::setup();

    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
}

[[maybe_unused]]
void loop() {
    auto optMsg = Midi::read();

//    if (optMsg.has_value()) {
//        std::visit([](auto &&msg) {
//            using T = std::decay_t<decltype(msg)>;
//            if constexpr (std::is_same_v<T, NoteOn>) {
//                Serial.println("Note On:");
//                Serial.println(msg.note);
//            } else if constexpr (std::is_same_v<T, NoteOff>) {
//                Serial.println("Note Off:");
//                Serial.println(msg.note);
//            } else if constexpr (std::is_same_v<T, Pressure>) {
//                Serial.println("Pressure:");
//                Serial.println(msg.note);
//            } else if constexpr (std::is_same_v<T, ControlChange>) {
//                Serial.println("Control Change:");
//                Serial.println(msg.control);
//            } else if constexpr (std::is_same_v<T, ProgramChange>) {
//                Serial.println("Program Change:");
//                Serial.println(msg.program);
//            }
//        }, optMsg.value());
//    }
}
