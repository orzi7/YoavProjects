#ifndef IMU_h
#define IMU_h

#include <Arduino.h>
#include <Wire.h>
#include <math.h>

class IMU {
  private:
    int MPU_ADDR;

  public:
    IMU();
    void init();
    float getAngle(); // Returns the calculated pitch angle
};

#endif