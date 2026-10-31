#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

class ServoController {
private:
    Adafruit_PWMServoDriver pwm;
    int currentPos[7];
    int targetPos[7];
    int moveDir[7];
    int servoPorts[7];
    unsigned long lastUpdate;
    const int STEP_DELAY = 15;
    const int STEP_SIZE = 4;

public:
    ServoController();
    void begin();
    void setDirection(int idx, int dir);
    void setTarget(int idx, int target);
    void update();
};

#endif