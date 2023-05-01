#ifndef __MONITORING_CAMERA_CAMERA_INTERFACE_H___
#define __MONITORING_CAMERA_CAMERA_INTERFACE_H___

#include <Arduino.h>
#include <esp_camera.h>

esp_err_t camera_setup (camera_config_t * config) {

    int CAM_PIN_PWDN = config->pin_pwdn;
    
    if(CAM_PIN_PWDN != -1){
        pinMode(CAM_PIN_PWDN, OUTPUT);
        digitalWrite(CAM_PIN_PWDN, LOW);
    }

    esp_err_t err = esp_camera_init(config);
    if (err!= ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
    }
    return err;
}

int camera_capture_frame(camera_fb_t * fb_target) {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.printf("Camera capture failed\n");
        return 0;
    }
    memcpy(fb_target, fb, sizeof(camera_fb_t));
    return 1;
}

#endif // __MONITORING_CAMERA_CAMERA_INTERFACE_H___