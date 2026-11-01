#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <Arduino.h>
#include "esp_camera.h"
#include <WebSocketsServer.h>

class CameraManager {

private:
    camera_config_t config;

public:
    CameraManager();
    void sendSingleFrame(WebSocketsServer& ws, uint8_t client_num);
};

#endif