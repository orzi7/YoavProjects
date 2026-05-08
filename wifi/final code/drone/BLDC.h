#ifndef BLDC_h
#define BLDC_h

#include <Arduino.h>
#include <Servo.h>

class BLDC {
  private:
    Servo rightServo;
    Servo leftServo;
    int servoRightPin;
    int servoLeftPin;
    
    int minSpeed;
    int midSpeed;
    int maxSpeed;
    float speeds_relation;

  public:
    BLDC(int rightPin, int leftPin);
    void init();
    // Takes the current angle and directly drives the motors
    void controlMotors(float currentAngle, float KP, int TARGET_ANGLE, int basicSpeed);
};

#endif