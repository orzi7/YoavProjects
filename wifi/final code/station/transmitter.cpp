#include <Arduino.h>
#include "transmitter.h"

transmitter::transmitter(int dataPin) : dataPin(dataPin), driver(2000, 9, dataPin, 10) {
}

void transmitter::init() {
  if (!driver.init()) Serial.println("init failed");
}

void transmitter::WriteData(String data) {
  const char *msg = data.c_str(); 
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
}
