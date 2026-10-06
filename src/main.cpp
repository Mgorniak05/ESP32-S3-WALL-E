#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "MotorController.h"
#include "secrets.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LittleFS.h>
#include "AudioPlayer.h"

#define I2C_SDA 21
#define I2C_SCL 47
#define AN2_MOTOR 46
#define PWMA_MOTOR 3
#define AN1_MOTOR 41
#define BN1_MOTOR 42
#define BN2_MOTOR 2
#define PWMB_MOTOR 1
#define DIN 14
#define BCLK 19
#define LRC 20
#define SD_DATA 40
#define SD_CLK 39
#define SD_CMD 38

WebSocketsServer webSocket = WebSocketsServer(81);
AudioPlayer walleAudio(BCLK, LRC, DIN);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);
MotorController robot(PWMA_MOTOR, AN1_MOTOR, AN2_MOTOR, PWMB_MOTOR, BN1_MOTOR, BN2_MOTOR);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected to website (IP: %d.%d.%d.%d)\n", num, ip[0], ip[1], ip[2], ip[3]);
            }
            break;
        case WStype_TEXT:
            String command = String((char*)payload);
            Serial.print("Otrzymano komendę z WWW: ");
            Serial.println(command);

            if (command == "Action1") {
                walleAudio.play("/1_WALL-E.mp3"); 
            } 
            else if (command == "Action2") {
                walleAudio.play("/2_EEEEE-VA.mp3");
            } 
            else if (command == "Action3") {
                walleAudio.play("/3_WALL-EEEEE.mp3");
            } 
            else if (command == "Action4") {
                walleAudio.play("/4_Hello Dolly.mp3");
            }
            else if (command == "Backward") {
                robot.goBack(128);
            }
            else if (command == "Forward") {
                robot.goForward(128);
            }
            else if (command == "Left") {
                robot.turnLeft(128);
            }
            else if (command == "Right") {
                robot.turnRight(128);
            }
            else if (command == "Stop") {
                robot.stop();
            }
            else {
                Serial.println("Nieznana komenda!");
                
            }
            
            break;
    }
}

void setup() {
  Serial.begin(115200);
  robot.begin();

    Serial.println("3.. ");
    delay(1000);
    Serial.println("2.. ");
    delay(1000);
    Serial.println("1.. ");
    delay(1000);
    Serial.println("Starting WebSocket Server...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("WiFi connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    webSocket.onEvent(webSocketEvent);
    webSocket.begin();

  if(!LittleFS.begin()){
    Serial.println("Error, LittleFS unable to mount!!!");
    return;
  }
  Serial.println("LittleFS mounted successfully.");

  walleAudio.begin();

}

void loop() {
  walleAudio.loop();
  webSocket.loop(); 

}