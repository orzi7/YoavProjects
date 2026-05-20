#include <Wire.h>
#include <ServoTimer2.h>
#include <math.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> 
#endif

RH_ASK driver(2000, 11, 4, 5);

ServoTimer2 rightServo;
ServoTimer2 leftServo;

const int MPU_ADDR = 0x68;

const int servoRightPin = 9;
const int servoLeftPin = 10;

const int minSpeed = 1100;
const int maxSpeed = 1800;

const int deadzone = 20;
const int middleJoystick = 430;

const float multification = 2200.0 / 2450.0;

int pwmValue = 1100;

int motorControl(String motor, float KP, int TARGET_ANGLE, int basicSpeed) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int pwmOutput = basicSpeed; 

  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    float angle = atan2(AcY, AcZ) * 180 / PI;

    float error = TARGET_ANGLE - angle;
    
    if (motor == "left") {
      int netSpeed = basicSpeed - minSpeed;
      pwmOutput = minSpeed + (netSpeed * multification) + (error * KP);
    }
    else if (motor == "right") {
      pwmOutput = basicSpeed - (error * KP);
    }

    if (pwmOutput < minSpeed) {
      pwmOutput = minSpeed;
    }

    if (pwmOutput > maxSpeed) {
      pwmOutput = maxSpeed;
    }

    return pwmOutput;
  }
  
  return pwmOutput; 
}

void setup() {
  Serial.begin(9600);
  if (!driver.init()) Serial.println("init failed");
  
  Wire.begin();
  Wire.setWireTimeout(3000, true); 
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);    
  Wire.endTransmission(true);

  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  rightServo.write(1100); 
  leftServo.write(1100); 
  delay(3000);
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; 
    pwmValue = atoi((char*)buf); 
  }

  Serial.println(pwmValue);
  if (pwmValue == 0) {
    rightServo.write(minSpeed);
    leftServo.write(minSpeed);
  } else {
    rightServo.write(motorControl("right", 1.2, 0, pwmValue));
    leftServo.write(motorControl("left", 1.2, 0, pwmValue));
  }
}
