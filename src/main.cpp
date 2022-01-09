#include "Arduino.h"

#include "midi/Midi.h"
#include "Pins.h"

void setup() {
    Midi::setup();

    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
}

void loop() {
    Midi::read();
}
