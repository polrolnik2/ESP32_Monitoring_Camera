#ifndef __MONITORING_CAMERA_CAMERA_INTERFACE_H___
#define __MONITORING_CAMERA_CAMERA_INTERFACE_H___

#include <Arduino.h>
#include <img_converters.h>
#include <esp_camera.h>
#include <esp_timer.h>

#define JPG_QUALITY 80

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

int camera_capture_frame(uint8_t * jpg_target, size_t jpg_target_size) {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.printf("Camera capture failed\n");
        return 0;
    }
    frame2jpg(fb, JPG_QUALITY, &jpg_target, &jpg_target_size);
    esp_camera_fb_return(fb);
    return 1;
}

#endif // __MONITORING_CAMERA_CAMERA_INTERFACE_H___