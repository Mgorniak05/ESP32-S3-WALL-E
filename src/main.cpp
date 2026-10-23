#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "MotorController.h"
#include "secrets.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LittleFS.h>
#include "AudioPlayer.h"
#include <Adafruit_INA219.h>

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

#define ARM_L_PORT 0
#define ARM_R_PORT 1
#define NECK_1_PORT 2
#define NECK_2_PORT 3
#define NECK_3_PORT 4
#define EYE_L_PORT 5
#define EYE_R_PORT 6

Adafruit_INA219 ina219;
WebSocketsServer webSocket = WebSocketsServer(81);
AudioPlayer walleAudio(BCLK, LRC, DIN);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x60);
MotorController robot(PWMA_MOTOR, AN1_MOTOR, AN2_MOTOR, PWMB_MOTOR, BN1_MOTOR, BN2_MOTOR);

int servoDirection = 0;
unsigned long lastServoUpdate = 0;
const int SERVO_PORTS[7] = {ARM_L_PORT, ARM_R_PORT, NECK_1_PORT, NECK_2_PORT, NECK_3_PORT, EYE_L_PORT, EYE_R_PORT};
int currentPos[7] = {307, 307, 307, 307, 307, 307, 307};
int whichServo = 1;
unsigned long lastTelemetry = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
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
        String command = String((char *)payload);
        Serial.print("Otrzymano komendę z WWW: ");
        Serial.println(command);
        webSocket.broadcastTXT("LOG:CMD -> " + command);

        if (command == "Action1")
        {
            walleAudio.play("/1_WALL-E.mp3");
            webSocket.broadcastTXT("LOG:PLAYING /1_WALL-E.mp3");
        }
        else if (command == "Action2")
        {
            walleAudio.play("/2_EEEEE-VA.mp3");
            webSocket.broadcastTXT("LOG:PLAYING /2_EEEEE-VA.mp3");
        }
        else if (command == "Action3")
        {
            walleAudio.play("/3_WALL-EEEEE.mp3");
            webSocket.broadcastTXT("LOG:PLAYING /3_WALL-EEEEE.mp3");
        }
        else if (command == "Action4")
        {
            walleAudio.play("/4_Hello Dolly.mp3");
            webSocket.broadcastTXT("LOG:PLAYING /4_Hello Dolly.mp3");
        }
        else if (command == "Forward")
        {
            robot.goBack(200);
            webSocket.broadcastTXT("DIR, FORWARD");
        }
        else if (command == "Backward")
        {
            robot.goForward(182);
            webSocket.broadcastTXT("DIR, BACKWARD");
        }
        else if (command == "Left")
        {
            robot.turnLeft(128);
            webSocket.broadcastTXT("DIR, LEFT");
        }
        else if (command == "Right")
        {
            robot.turnRight(128);
            webSocket.broadcastTXT("DIR, RIGHT");
        }
        else if (command == "Stop")
        {
            robot.stop();
            webSocket.broadcastTXT("DIR, STOP");
        }
        else if (command.startsWith("Servo") && command.length() == 6)
        {
            int val = command.substring(5).toInt();
            if (val >= 1 && val <= 7)
            {
                whichServo = val;
                Serial.printf("Wybrano serwo nr: %d\n", whichServo);
                webSocket.broadcastTXT("LOG:SERVO " + String(whichServo) + " SELECTED");
            }
        }
        else if (command == "ServoLeft")
        {
            servoDirection = -1;
            webSocket.broadcastTXT("LOG:Q PRESSED");
        }
        else if (command == "ServoRight")
        {
            servoDirection = 1;
            webSocket.broadcastTXT("LOG:E PRESSED");
        }
        else if (command == "ServoStop")
        {
            servoDirection = 0;
            webSocket.broadcastTXT("LOG:SERVO STOPPED");
        }
        else if (command == "ServoCenter")
        {
            int idx = whichServo - 1;
            currentPos[idx] = 307;
            pwm.setPWM(SERVO_PORTS[idx], 0, 307);
            webSocket.broadcastTXT("LOG:SERVO " + String(whichServo) + " CENTERED");
        }
        else
        {
            Serial.println("Unknown command!");
            webSocket.broadcastTXT("LOG:UNKNOWN COMMAND");
        }
        break;
    }
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setTimeOut(50);

    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(50);
    Serial.println("Servo driver initialized.");

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
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");

    if (!ina219.begin())
    {
        Serial.println("INA219 not found");
    }
    else
    {
        Serial.println("INA219 initialized.");
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    webSocket.onEvent(webSocketEvent);
    webSocket.begin();

    if (!LittleFS.begin())
    {
        Serial.println("Error, LittleFS unable to mount!!!");
        return;
    }
    Serial.println("LittleFS mounted successfully.");

    walleAudio.begin();
}

void loop()
{
    walleAudio.loop();
    webSocket.loop();
    if (servoDirection != 0 && (millis() - lastServoUpdate > 20))
    {
        lastServoUpdate = millis();

        int idx = whichServo - 1;
        currentPos[idx] += servoDirection;

        if (currentPos[idx] < 50)
            currentPos[idx] = 50;
        if (currentPos[idx] > 600)
            currentPos[idx] = 600;

        pwm.setPWM(SERVO_PORTS[idx], 0, currentPos[idx]);
    }

    if (millis() - lastTelemetry > 1000)
    {
        lastTelemetry = millis();

        float voltage = ina219.getBusVoltage_V();
        float current = ina219.getCurrent_mA();

        String telemetryMsg = "TELE," + String(voltage) + "," + String(current);
        webSocket.broadcastTXT(telemetryMsg);
    }
}