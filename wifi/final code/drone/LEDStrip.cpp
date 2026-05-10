#include "LEDStrip.h"

LEDStrip::LEDStrip(int rPin, int gPin, int bPin) : pinR(rPin), pinG(gPin), pinB(bPin) {
}

void LEDStrip::init() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  
  turnOff(); 
}

void LEDStrip::setColor(int r, int g, int b) {
  // Common Anode logic: We want 255 to be full brightness (LOW) 
  // and 0 to be completely off (HIGH).
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  analogWrite(pinR, 255 - r);
  analogWrite(pinG, 255 - g);
  analogWrite(pinB, 255 - b);
}

void LEDStrip::turnOff() {
  // For a Common Anode strip, outputting HIGH means 0 potential difference, 
  // so the LEDs turn OFF.
  digitalWrite(pinR, HIGH);
  digitalWrite(pinG, HIGH);
  digitalWrite(pinB, HIGH);
}