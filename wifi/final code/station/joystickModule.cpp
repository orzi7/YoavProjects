#include <Arduino.h>
#include "joystickModule.h"

joystickModule::joystickModule(int xAxis, int yAxis, int SW, int minSpeed, int maxSpeed) {
  this->xAxis = xAxis;
  this->yAxis = yAxis;
  this->SW = SW;
  this->minSpeed = minSpeed;
  this->maxSpeed = maxSpeed;
}

void joystickModule::init() {
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  pinMode(SW, INPUT);

}

long joystickModule::getRawXPose() {
  return analogRead(xAxis);
}

long joystickModule::getRawYPose() {
  return analogRead(yAxis);
}


long joystickModule::getMappedXPose() {
  return map(getRawXPose(), 0, 1023, this->minSpeed, this->maxSpeed);
}

long joystickModule::getMappedYPose() {
  return map(getRawYPose(), 0, 1023, this->minSpeed, this->maxSpeed);
}

int joystickModule::getSW() {
  return digitalRead(SW);
}
