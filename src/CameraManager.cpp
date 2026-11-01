#include "CameraManager.h"

CameraManager::CameraManager()
{
    memset(&config, 0, sizeof(camera_config_t));
    camera_config_t c;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 11;
    config.pin_d1 = 9;
    config.pin_d2 = 8;
    config.pin_d3 = 10;
    config.pin_d4 = 12;
    config.pin_d5 = 18;
    config.pin_d6 = 17;
    config.pin_d7 = 16;
    config.pin_xclk = 15;
    config.pin_pclk = 13;
    config.pin_vsync = 6;
    config.pin_href = 7;
    config.pin_sccb_sda = 4;
    config.pin_sccb_scl = 5;
    config.pin_pwdn = -1;
    config.pin_reset = -1;
    config.xclk_freq_hz = 10000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
}

void CameraManager::sendSingleFrame(WebSocketsServer &ws, uint8_t client_num)
{
    ws.sendTXT(client_num, "LOG:Waking up camera...");

    if (esp_camera_init(&config) != ESP_OK)
    {
        ws.sendTXT(client_num, "LOG:Camera init error!");
        return;
    }

    for (int i = 0; i < 2; i++)
    {
        camera_fb_t *fb_dummy = esp_camera_fb_get();
        if (fb_dummy)
        {
            esp_camera_fb_return(fb_dummy);
        }
        delay(40);
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (fb)
    {
        ws.sendBIN(client_num, fb->buf, fb->len);
        esp_camera_fb_return(fb);
        ws.sendTXT(client_num, "LOG:Picture sent to client");
    }
    else
    {
        ws.sendTXT(client_num, "LOG:Camera error - empty frame!");
    }

    esp_camera_deinit();
    ws.sendTXT(client_num, "LOG:Camera sleeping");
}