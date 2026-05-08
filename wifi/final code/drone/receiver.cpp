#include <Arduino.h>
#include "receiver.h"

receiver::receiver(int dataPin) : dataPin(dataPin), driver(2000, dataPin, 9, 10) {
}

void receiver::init() {
  if (!driver.init()) Serial.println("init failed");
}

String receiver::readData() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    String message = "";

    for (int i = 0; i < buflen; i++) {
      message += (char)buf[i];
    }
        
    Serial.println(message);
    return message;
  }
  return "NA";
}
