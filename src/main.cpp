#include <Arduino.h>

#include "led/LEDControl.h"
#include "midi/Midi.h"

[[maybe_unused]]
void setup() {
    Serial.begin(115200);

    LEDControl::setup();

    Midi::setup();
}

[[maybe_unused]]
void loop() {
    auto optMsg = Midi::read();

    if (optMsg.has_value()) {
        LEDControl::handleMidi(optMsg.value());
    }

    LEDControl::update();
}
