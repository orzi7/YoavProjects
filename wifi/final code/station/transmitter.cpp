#include <Arduino.h>
#include "transmitter.h"

transmitter::transmitter(int dataPin) : dataPin(dataPin), driver(4000, 9, dataPin, 10) {
}

void transmitter::init() {
  if (!driver.init()) Serial.println("init failed");
}
