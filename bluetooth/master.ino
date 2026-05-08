int state = 0;
const int Yport = A0;

int Yvalue;
int rawYvalue;

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1);

const int middleJoystick = 430;
const int deadzone = 100;

const int minSpeed = 1100;
const int maxSpeed = 1900;

int pwmValue;

void setup() {
  BTSerial.begin(38400);
}

void loop() {
  rawYvalue = analogRead(Yport);
  if (rawYvalue < (middleJoystick - deadzone)) {
    pwmValue = map(rawYvalue, middleJoystick - deadzone, 0, minSpeed, maxSpeed);
  } else if (rawYvalue > (middleJoystick + deadzone)) {
    pwmValue = map(rawYvalue, middleJoystick + deadzone, 1023, minSpeed, maxSpeed);
  } else {
    pwmValue = minSpeed;
  }

  state = pwmValue;

  Serial.println(state);

  BTSerial.println(state);
}
