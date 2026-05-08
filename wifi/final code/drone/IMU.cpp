#include "IMU.h"

IMU::IMU() : MPU_ADDR(0x68) {
}

void IMU::init() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

float IMU::getAngle() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // Request 6 registers

  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    // Calculate and return the angle
    return atan2(AcY, AcZ) * 180 / PI;
  }
  
  return 0.0; // Safe fallback if sensor read fails
}