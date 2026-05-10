#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>

class LEDStrip {
  private:
    int pinR;
    int pinG;
    int pinB;

  public:
    LEDStrip(int rPin, int gPin, int bPin);
    void init();
    void setColor(int r, int g, int b); 
    void turnOff(); 
};

#endif