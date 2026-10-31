#include "CameraManager.h"

bool CameraManager::begin() {
    camera_config_t c;
    c.ledc_channel = LEDC_CHANNEL_0; 
    c.ledc_timer = LEDC_TIMER_0;
    c.pin_d0 = 11; c.pin_d1 = 9;  c.pin_d2 = 8;  c.pin_d3 = 10;
    c.pin_d4 = 12; c.pin_d5 = 18; c.pin_d6 = 17; c.pin_d7 = 16;
    c.pin_xclk = 15; c.pin_pclk = 13; c.pin_vsync = 6; c.pin_href = 7;
    c.pin_sccb_sda = 4; c.pin_sccb_scl = 5; 
    c.pin_pwdn = -1; c.pin_reset = -1;
    c.xclk_freq_hz = 10000000;
    c.pixel_format = PIXFORMAT_JPEG;
    c.frame_size = FRAMESIZE_QVGA; 
    c.jpeg_quality = 20; 
    c.fb_count = 1;
    c.fb_location = CAMERA_FB_IN_PSRAM;

    if (esp_camera_init(&c) != ESP_OK) {
        Serial.println("Camera initialization error!");
        return false;
    }
    return true;
}

void CameraManager::sendSingleFrame(WebSocketsServer& ws, uint8_t client_num) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
        esp_camera_fb_return(fb); 
    }

    fb = esp_camera_fb_get();
    if (fb) {
        ws.sendBIN(client_num, fb->buf, fb->len);
        esp_camera_fb_return(fb);
    } else {
        ws.sendTXT(client_num, "LOG:Camera error- empty frame!");
    }
}