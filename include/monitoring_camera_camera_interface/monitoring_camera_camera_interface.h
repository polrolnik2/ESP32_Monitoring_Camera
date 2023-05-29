#ifndef __MONITORING_CAMERA_CAMERA_INTERFACE_H___
#define __MONITORING_CAMERA_CAMERA_INTERFACE_H___

// ------------------------------------------------------------------------------------------------
// Methods for initializing the camera and capturing image in the correct JPEG format.
// ------------------------------------------------------------------------------------------------

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

esp_err_t camera_capture_frame(uint8_t ** jpg_target, size_t * jpeg_len) {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.printf("Camera capture failed\n");
        return ESP_FAIL;
    }
    *jpg_target = new uint8_t[fb->len];
    memcpy(*jpg_target, fb->buf, fb->len);
    *jpeg_len = fb->len;
    esp_camera_fb_return(fb);
    return ESP_OK;
}

#endif // __MONITORING_CAMERA_CAMERA_INTERFACE_H___