#include <Arduino.h>
#include <ESP32Servo.h>

const int servoPin = 4;
Servo servoNumberOne;
void setup(){
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servoNumberOne.setPeriodHertz(50);
  servoNumberOne.attach(servoPin, 500, 2400);
}

void loop(){
  Serial.println("Moving to 180 degrees");
  for(int pos = 0; pos <= 180; pos++){
    servoNumberOne.write(pos);
    delay(15);
  }
  Serial.println("Moving to 0 degrees");
  for(int pos = 180; pos >= 0; pos--){
    servoNumberOne.write(pos);
    delay(15);
  }
  Serial.println("Finished");
}