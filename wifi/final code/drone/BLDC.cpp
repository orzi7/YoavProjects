#include "BLDC.h"

BLDC::BLDC(int rightPin, int leftPin) : 
  servoRightPin(rightPin), 
  servoLeftPin(leftPin), 
  minSpeed(1000), 
  midSpeed(1500), 
  maxSpeed(2000), 
  speeds_relation(2200.0 / 2450.0) {
}

void BLDC::init() {
  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  // Brushless Motor / ESC Calibration Sequence
  rightServo.write(1000); 
  leftServo.write(1000); 
  delay(3000);
  // rightServo.write(1500); 
  // leftServo.write(1500); 
  // delay(100);
  // rightServo.write(1900); 
  // leftServo.write(1900); 
  // delay(500);
  // rightServo.write(1500); 
  // leftServo.write(1500); 
  // delay(500);
  // rightServo.write(1100); 
  // leftServo.write(1100); 
  // delay(500);
}

void BLDC::controlMotors(float currentAngle, float KP, int TARGET_ANGLE, int basicSpeed) {
  float error = TARGET_ANGLE - currentAngle;
  
  // Calculate PWM for both sides
  int leftPwm = (speeds_relation * basicSpeed) + (error * KP);
  int rightPwm = basicSpeed - (error * KP);

  // Constrain left motor speed
  if (leftPwm < minSpeed) leftPwm = minSpeed;
  if (leftPwm > maxSpeed) leftPwm = maxSpeed;
  
  // Constrain right motor speed
  if (rightPwm < minSpeed) rightPwm = minSpeed;
  if (rightPwm > maxSpeed) rightPwm = maxSpeed;

  // Write directly to the ESCs
  leftServo.write(leftPwm);
  rightServo.write(rightPwm);
}

void BLDC::rightMotor(int basicSpeed) {

  // Constrain right motor speed
  if (basicSpeed < minSpeed) basicSpeed = minSpeed;
  if (basicSpeed > maxSpeed) basicSpeed = maxSpeed;

  // Write directly to the ESCs
  rightServo.write(basicSpeed);
}

void BLDC::leftMotor(int basicSpeed) {

  // Constrain right motor speed
  if (basicSpeed < minSpeed) basicSpeed = minSpeed;
  if (basicSpeed > maxSpeed) basicSpeed = maxSpeed;

  // Write directly to the ESCs
  leftServo.write(basicSpeed);
}