#ifndef transmitter_h
#define transmitter_h

#include <Arduino.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

class transmitter {
  private:
    int dataPin;
    RH_ASK driver;
  public:
    transmitter(int dataPin);
    void init();
    void WriteData(String stringData);

};

#endif