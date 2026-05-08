#include <Wire.h>
#include <Servo.h>
#include <math.h>

Servo rightServo;
Servo leftServo;

const int MPU_ADDR = 0x68;

const int servoRightPin = 9;
const int servoLeftPin = 10;

const int minSpeed = 1100;
const int midSpeed = 1500;
const int maxSpeed = 1700;

int pwmOutput;

const float multification = 2200.0 / 2450.0;

int motorControl(String motor, float KP, int TARGET_ANGLE, int basicSpeed) {
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
    
    if (motor == "left") {
      pwmOutput = (multification * basicSpeed) - (error * KP);
    }
    else if (motor == "right") {
      pwmOutput = basicSpeed + (error * KP);
    }

    if (pwmOutput < minSpeed) {
      pwmOutput = minSpeed;
    }

    if (pwmOutput > maxSpeed) {
      pwmOutput = maxSpeed;
    }

    Serial.println(angle);

    return pwmOutput;
  }
}

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

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

  pinMode(13, HIGH);
  delay(1000);
}

void loop() {
  rightServo.writeMicroseconds(motorControl("right", 5, 0, midSpeed));
  leftServo.writeMicroseconds(motorControl("left", 5, 0, midSpeed));
}
