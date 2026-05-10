#ifndef receiver_h
#define receiver_h

#include <Arduino.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

class receiver {
  private:
    int dataPin;
    RH_ASK driver;
  public:
    receiver(int dataPin);
    void init();
    String readRawData();
    String getValue(String data, char separator, int index);
};

#endif