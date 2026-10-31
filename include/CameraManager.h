#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <Arduino.h>
#include "esp_camera.h"
#include <WebSocketsServer.h>

class CameraManager {
public:
    bool begin();
    void sendSingleFrame(WebSocketsServer& ws, uint8_t client_num);
};

#endif