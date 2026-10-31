#include "ServoController.h"

ServoController::ServoController() : pwm(0x60) {
    int initialPorts[7] = {0, 1, 2, 3, 4, 5, 6};
    for(int i = 0; i < 7; i++) {
        servoPorts[i] = initialPorts[i];
        currentPos[i] = 307;
        targetPos[i] = 307;
        moveDir[i] = 0;
    }
    lastUpdate = 0;
}

void ServoController::begin() {
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(50);
    for(int i = 0; i < 7; i++) {
        pwm.setPWM(servoPorts[i], 0, currentPos[i]);
    }
}

void ServoController::setDirection(int idx, int dir) {
    moveDir[idx] = dir;
    if (dir == 0) {
        targetPos[idx] = currentPos[idx]; 
    }
}

void ServoController::setTarget(int idx, int target) {
    if(target < 50) target = 50;
    if(target > 600) target = 600;
    targetPos[idx] = target;
    moveDir[idx] = 0; 
}

void ServoController::update() {
    if (millis() - lastUpdate > STEP_DELAY) {
        lastUpdate = millis();
        
        for (int i = 0; i < 7; i++) {
            if (moveDir[i] != 0) {
                targetPos[i] = currentPos[i] + (moveDir[i] * STEP_SIZE * 2);
                if(targetPos[i] < 50) targetPos[i] = 50;
                if(targetPos[i] > 600) targetPos[i] = 600;
            }

            if (currentPos[i] != targetPos[i]) {
                if (currentPos[i] < targetPos[i]) {
                    currentPos[i] += STEP_SIZE;
                    if (currentPos[i] > targetPos[i]) currentPos[i] = targetPos[i];
                } else {
                    currentPos[i] -= STEP_SIZE;
                    if (currentPos[i] < targetPos[i]) currentPos[i] = targetPos[i];
                }
                pwm.setPWM(servoPorts[i], 0, currentPos[i]);
            }
        }
    }
}