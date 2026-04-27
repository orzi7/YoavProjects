#include <Servo.h>

Servo rightServo;
Servo leftServo;

const int joystickYPin = A0;
const int servoRightPin = 9;
const int servoLeftPin = 10;

int pwmValue;

void setup() {
  Serial.begin(9600);
  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  // stopValue ~1100, midSpeedFwd ~1500, maxSpeedFwd ~1900
  rightServo.writeMicroseconds(1500); // MIDDLE
  leftServo.writeMicroseconds(1500); // MIDDLE
  delay(100);
  rightServo.writeMicroseconds(1900); // Max Forward
  leftServo.writeMicroseconds(1900); // Max Forward
  delay(500);
  rightServo.writeMicroseconds(1500); // MIDDLE
  leftServo.writeMicroseconds(1500); // MIDDLE
  delay(500);
  rightServo.writeMicroseconds(1100); // STOP
  leftServo.writeMicroseconds(1100); // STOP
  delay(500);
}

void loop() {
  int joyValue = analogRead(joystickYPin);
  
  if (joyValue < 500) {
    pwmValue = map(joyValue, 500, 0, 1100, 1900);
  } else if (joyValue > 550) {
    pwmValue = map(joyValue, 550, 1023, 1100, 1900);
  } else {
    pwmValue = 1100;
  }
  
  rightServo.writeMicroseconds(pwmValue);
  leftServo.writeMicroseconds(pwmValue);
  
  Serial.print("Joystick Y: ");
  Serial.print(joyValue);
  Serial.print(" --> PWM: ");
  Serial.println(pwmValue);
}
