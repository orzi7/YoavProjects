#include "IMU.h"
#include "BLDC.h"
#include "LEDStrip.h"

// Instantiate your objects
IMU myIMU;
BLDC myMotors(9, 10); // Right motor on Pin 9, Left motor on Pin 10
LEDStrip myLEDs(3, 5, 11)

// Balancing Constants
const int midSpeed = 1500;
const float KP = 5.0;
const int TARGET_ANGLE = 0;

void setup() {
  Serial.begin(9600);
  myLEDs.setColor(255, 0, 0);
  
  myIMU.init();    // Wakes up MPU6050
  myMotors.init(); // Attaches servos and runs ESC calibration
  myLEDs.init();

  myLEDs.setColor(0, 255, 0);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
}

void loop() {
  float currentAngle = myIMU.getAngle();
  
  // for debugging
  Serial.println(currentAngle);
  
  // to balance the system
  myMotors.controlMotors(currentAngle, KP, TARGET_ANGLE, midSpeed);
}