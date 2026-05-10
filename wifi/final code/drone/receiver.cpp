#include <Arduino.h>
#include "receiver.h"

receiver::receiver(int dataPin) : dataPin(dataPin), driver(4000, dataPin, 9, 10) {
}

void receiver::init() {
  if (!driver.init()) Serial.println("init failed");
}

String receiver::readRawData() {
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

String receiver::getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
