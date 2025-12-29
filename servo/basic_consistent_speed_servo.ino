#include <Wire.h>
#include <Servo.h>
#include <math.h>

Servo myServo;
const int MPU_ADDR = 0x68;

const int MOTOR_PIN = 9;

void motorControl(float KP, int TARGET_ANGLE) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    float angle = atan2(AcY, AcZ) * 180 / PI;

    float error = TARGET_ANGLE - angle;
    
    int pwmOutput = 1500 + (error * KP);

    pwmOutput = constrain(pwmOutput, 1100, 1900);

    myServo.writeMicroseconds(pwmOutput);

    Serial.print("Angle: "); Serial.print(angle);
    Serial.print(" | Error: "); Serial.print(error);
    Serial.print(" | Motor PWM: "); Serial.println(pwmOutput);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  myServo.attach(MOTOR_PIN);
  myServo.writeMicroseconds(1500);
  delay(100);
  myServo.writeMicroseconds(1900);
  delay(500);
  myServo.writeMicroseconds(1100);
  delay(500);
  myServo.writeMicroseconds(1500);
  delay(500);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  motorControl(15, 0);
}
