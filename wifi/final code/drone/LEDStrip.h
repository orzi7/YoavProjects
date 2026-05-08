#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>

class LEDStrip {
  private:
    int pinR;
    int pinG;
    int pinB;

  public:
    // Constructor takes the three Arduino pins connected to your MOSFETs
    LEDStrip(int rPin, int gPin, int bPin);
    
    // Sets up the pins
    void init();
    
    // Mixes a custom color using standard 0-255 RGB values
    void setColor(int r, int g, int b); 
    
    // Quick helper to shut the strip off completely
    void turnOff(); 
};

#endif