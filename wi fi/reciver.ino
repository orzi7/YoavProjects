#include <Wire.h>
#include <ServoTimer2.h>
#include <math.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 2, 9, 10);

ServoTimer2 rightServo;
ServoTimer2 leftServo;

const int MPU_ADDR = 0x68;

const int joystickYPin = A0;
const int servoRightPin = 9;
const int servoLeftPin = 10;

const int minSpeed = 1100;
const int maxSpeed = 1700;

const int deadzone = 20;
const int middleJoystick = 430;

const float multification = 2200.0 / 2450.0;

int pwmValue;

int motorControl(String motor, float KP, int TARGET_ANGLE, int basicSpeed) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int pwmOutput;

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

    return pwmOutput;
  }
}

void setup() {
  Serial.begin(9600);
  if (!driver.init()) Serial.println("init failed");
  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  // stopValue ~1100, midSpeedFwd ~1500, maxSpeedFwd ~1900
  rightServo.write(1500); // MIDDLE
  leftServo.write(1500); // MIDDLE
  delay(100);
  rightServo.write(1900); // Max Forward
  leftServo.write(1900); // Max Forward
  delay(500);
  rightServo.write(1500); // MIDDLE
  leftServo.write(1500); // MIDDLE
  delay(500);
  rightServo.write(1100); // STOP
  leftServo.write(1100); // STOP
  delay(500);
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) {

      String str = "";

      for (int i = 0; i < buflen; i++) {
        str += (char)buf[i];
      }
    
      pwmValue = str.toInt();
    }

  rightServo.write(motorControl("right", 5, 0, pwmValue));
  leftServo.write(motorControl("left", 5, 0, pwmValue));
}
