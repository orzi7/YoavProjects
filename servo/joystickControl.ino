#include <Servo.h>

Servo myServo;
const int joystickYPin = A0;

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  
  // stopValue ~1100, midSpeedFwd ~1500, maxSpeedFwd ~1900
  myServo.writeMicroseconds(1500); // Stop
  delay(100);
  myServo.writeMicroseconds(1900); // Max Forward
  delay(500);
  myServo.writeMicroseconds(1100); // NO SPEED
  delay(500);
  myServo.writeMicroseconds(1500); // MIDDLE
  delay(500);
}

void loop() {
  int joyValue = analogRead(joystickYPin);
  
  int pwmValue = map(joyValue, 0, 1023, 1100, 1900);
  
  myServo.writeMicroseconds(pwmValue);
  Serial.print("Joystick Y: ");
  Serial.print(joyValue);
  Serial.print("  -->  PWM: ");
  Serial.println(pwmValue);
  
}
