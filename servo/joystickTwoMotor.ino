#include <Servo.h>

Servo rightServo;
Servo lefttServo;

const int joystickYPin = A0;
const int servoRightPin = 9;
const int servoLeftPin = 10;

void setup() {
  Serial.begin(9600);
  rightServo.attach(servoRightPin);
  lefttServo.attach(servoRightPin);
  
  // stopValue ~1100, midSpeedFwd ~1500, maxSpeedFwd ~1900
  rightServo.writeMicroseconds(1500); // MIDDLE
  lefttServo.writeMicroseconds(1500); // MIDDLE
  delay(100);
  rightServo.writeMicroseconds(1900); // Max Forward
  lefttServo.writeMicroseconds(1900); // Max Forward
  delay(500);
  rightServo.writeMicroseconds(1500); // MIDDLE
  lefttServo.writeMicroseconds(1500); // MIDDLE
  delay(500);
  rightServo.writeMicroseconds(1100); // STOP
  lefttServo.writeMicroseconds(1100); // STOP
  delay(500);
}

void loop() {
  int joyValue = analogRead(joystickYPin);
  
  int pwmValue = map(joyValue, 0, 1023, 1100, 1900);
  
  rightServo.writeMicroseconds(pwmValue);
  lefttServo.writeMicroseconds(pwmValue);
  Serial.print("Joystick Y: ");
  Serial.print(joyValue);
  Serial.print("  -->  PWM: ");
  Serial.println(pwmValue);
  
}
