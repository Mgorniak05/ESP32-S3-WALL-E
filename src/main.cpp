#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <MotorController.h>
#include <secrets.h>

const char* nameWiFi = WIFI_SSID;
const char* passWiFi = WIFI_PASSWORD;

WebSocketsServer webSocket = WebSocketsServer(81);

MotorController robot(4, 5, 6, 7, 15, 16);

void onEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
        String receivedCommand = (char*)payload;
        
        Serial.println(receivedCommand);

        if (receivedCommand == "Forward")
          robot.goForward(128);
        else if (receivedCommand == "Backward")
          robot.goBack(128);
        else if (receivedCommand == "Left")
          robot.turnLeft(128);
        else if (receivedCommand == "Right")
          robot.turnRight(128);
        else if (receivedCommand == "Boost")
          robot.goForward(255);
        else if (receivedCommand == "Stop")
          robot.stop();
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

    WiFi.begin(nameWiFi, passWiFi);
    Serial.print("WiFi connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    webSocket.begin();
    webSocket.onEvent(onEvent);
    
}

void loop() {
    webSocket.loop(); 
}