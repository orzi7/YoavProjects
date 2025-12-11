#include <Servo.h>
Servo myServo;
int speedValue = 0

void setup() {
  myServo.attach(9);
  //stopValue ~1500, maxSpeedFwd ~1900, maxSpeedRev ~1100
  myServo.writeMicroseconds(1500);
  delay(100);
  myServo.writeMicroseconds(1900);
  delay(500);
  myServo.writeMicroseconds(1100);
  delay(500);
  myServo.writeMicroseconds(1500);
  delay(500);
}

void loop() {
  speedValue = 50;
  int pwmValue = map(speedValue, -100, 100, 1100, 1900);
  myServo.writeMicroseconds(pwmValue);
  delay(2000);
  myServo.writeMicroseconds(1500);
  delay(2000);
}
