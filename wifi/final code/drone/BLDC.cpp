#include "BLDC.h"

// Initialize pins and constants using the Member Initializer List
BLDC::BLDC(int rightPin, int leftPin) : 
  servoRightPin(rightPin), 
  servoLeftPin(leftPin), 
  minSpeed(1100), 
  midSpeed(1500), 
  maxSpeed(1700), 
  speeds_relation(2200.0 / 2450.0) {
}

void BLDC::init() {
  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  // Brushless Motor / ESC Calibration Sequence
  rightServo.writeMicroseconds(1500); 
  leftServo.writeMicroseconds(1500); 
  delay(100);
  rightServo.writeMicroseconds(1900); 
  leftServo.writeMicroseconds(1900); 
  delay(500);
  rightServo.writeMicroseconds(1500); 
  leftServo.writeMicroseconds(1500); 
  delay(500);
  rightServo.writeMicroseconds(1100); 
  leftServo.writeMicroseconds(1100); 
  delay(500);
}

void BLDC::controlMotors(float currentAngle, float KP, int TARGET_ANGLE, int basicSpeed) {
  float error = TARGET_ANGLE - currentAngle;
  
  // Calculate PWM for both sides
  int leftPwm = (speeds_relation * basicSpeed) - (error * KP);
  int rightPwm = basicSpeed + (error * KP);

  // Constrain left motor speed
  if (leftPwm < minSpeed) leftPwm = minSpeed;
  if (leftPwm > maxSpeed) leftPwm = maxSpeed;
  
  // Constrain right motor speed
  if (rightPwm < minSpeed) rightPwm = minSpeed;
  if (rightPwm > maxSpeed) rightPwm = maxSpeed;

  // Write directly to the ESCs
  leftServo.writeMicroseconds(leftPwm);
  rightServo.writeMicroseconds(rightPwm);
}