#ifndef COMBOCONTROLLER_H
#define COMBOCONTROLLER_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "MotorController.h"
#include "AudioPlayer.h"
#include "ServoController.h"

class ComboController {
private:
    MotorController* robot;
    AudioPlayer* audio;
    ServoController* servos;
    WebSocketsServer* ws;

    int activeCombo;
    
    int servoStep;
    unsigned long servoTimer;
    
    int motorStep;
    unsigned long motorTimer;

    void centerAllServos();
    void finishCombo();

public:
    ComboController(MotorController* r, AudioPlayer* a, ServoController* s, WebSocketsServer* w);
    void startCombo(int comboId);
    void update();
};

#endif