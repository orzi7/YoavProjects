#ifndef BLDC_h
#define BLDC_h

#include <Arduino.h>
#include <ServoTimer2.h>

class BLDC {
  private:
    ServoTimer2 rightServo;
    ServoTimer2 leftServo;
    int servoRightPin;
    int servoLeftPin;
    
    int minSpeed;
    int midSpeed;
    int maxSpeed;
    float speeds_relation;

  public:
    BLDC(int rightPin, int leftPin);
    void init();
    void controlMotors(float currentAngle, float KP, int TARGET_ANGLE, int basicSpeed);
};

#endif