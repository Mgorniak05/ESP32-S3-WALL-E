#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class MotorController {
private:
    int pinPWMA, pinAIN1, pinAIN2;
    int pinPWMB, pinBIN1, pinBIN2;

public:
    MotorController(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2);

    void begin();

    void goForward(int speed);
    void goBack(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stop();
};

#endif