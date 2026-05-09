#include "MotorController.h"

MotorController::MotorController(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2) {
    pinPWMA = pwma;
    pinAIN1 = ain1;
    pinAIN2 = ain2;
    pinPWMB = pwmb;
    pinBIN1 = bin1;
    pinBIN2 = bin2;
}

void MotorController::begin() {
    pinMode(pinPWMA, OUTPUT);
    pinMode(pinAIN1, OUTPUT);
    pinMode(pinAIN2, OUTPUT);
    
    pinMode(pinPWMB, OUTPUT);
    pinMode(pinBIN1, OUTPUT);
    pinMode(pinBIN2, OUTPUT);

    stop();
}

void MotorController::goForward(int speed) {
    digitalWrite(pinAIN1, HIGH);
    digitalWrite(pinAIN2, LOW);
    analogWrite(pinPWMA, speed);
    
    digitalWrite(pinBIN1, HIGH);
    digitalWrite(pinBIN2, LOW);
    analogWrite(pinPWMB, speed);
}

void MotorController::goBack(int speed) {
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, HIGH);
    analogWrite(pinPWMA, speed);
    
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, HIGH);
    analogWrite(pinPWMB, speed);
}

void MotorController::turnLeft(int speed) {
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, HIGH);
    analogWrite(pinPWMA, speed);
    
    digitalWrite(pinBIN1, HIGH);
    digitalWrite(pinBIN2, LOW);
    analogWrite(pinPWMB, speed);
}

void MotorController::turnRight(int speed) {
    digitalWrite(pinAIN1, HIGH);
    digitalWrite(pinAIN2, LOW);
    analogWrite(pinPWMA, speed);
    
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, HIGH);
    analogWrite(pinPWMB, speed);
}

void MotorController::stop() {
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, LOW);
    analogWrite(pinPWMA, 0);
    
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, LOW);
    analogWrite(pinPWMB, 0);
}