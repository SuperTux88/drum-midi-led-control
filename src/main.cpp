#include "Arduino.h"

#include "midi.h"

#define LED_PIN 5

void setup() {
    setupMidi();

    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
}

void loop() {
    readMidi();
}
