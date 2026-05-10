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

    template<typename... Args>
    void writeData(const Args&... args) {
      String finalData;
      finalData.reserve(64);

      int unpacker[] = { 0, (finalData += String(args), 0)... };
      
      (void)unpacker; 

      driver.send((uint8_t *)finalData.c_str(), finalData.length());
      driver.waitPacketSent();
    }

};

#endif