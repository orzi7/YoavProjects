#ifndef joystickModule_h
#define joystickModule_h

#include <Arduino.h>

class joystickModule {
  private:
    int xAxis;
    int yAxis;
    int SW;
    int minSpeed;
    int maxSpeed;

  public:
    joystickModule(int xAxis, int yAxis, int SW, int minSpeed, int maxSpeed);
    void init();
    long getRawXPose();
    long getRawYPose();
    long getMappedXPose();
    long getMappedYPose();
    int getSW();


};

#endif