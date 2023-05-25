#ifndef __MONITORING_CAMERA_CAMERA_INTERFACE_H___
#define __MONITORING_CAMERA_CAMERA_INTERFACE_H___

#include <Arduino.h>
#include <img_converters.h>
#include <esp_camera.h>
#include <esp_timer.h>

#define JPG_QUALITY 80

esp_err_t camera_setup (camera_config_t * config) {

    int cam_pin_pwdn = config->pin_pwdn;
    
    if(cam_pin_pwdn != -1){
        pinMode(cam_pin_pwdn, OUTPUT);
        digitalWrite(cam_pin_pwdn, LOW);
    }

    esp_err_t err = esp_camera_init(config);
    if (err!= ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
    }
    return err;
}

esp_err_t camera_capture_frame(uint8_t ** jpg_target, size_t * jpg_target_size) {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.printf("Camera capture failed\n");
        return ESP_FAIL;
    }
    bool err = frame2jpg(fb, JPG_QUALITY, jpg_target, jpg_target_size);
    if(!err){
      Serial.println("JPEG compression failed");
      return ESP_FAIL;
    }
    esp_camera_fb_return(fb);
    return ESP_OK;
}

#endif // __MONITORING_CAMERA_CAMERA_INTERFACE_H___