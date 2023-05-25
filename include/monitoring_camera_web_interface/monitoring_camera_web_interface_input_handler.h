#ifndef __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___
#define __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___

#include <WiFi.h>
#include <esp_http_server.h>
#include <monitoring_camera_servo_control/monitoring_camera_servo_control.h>

monitoring_camera_servo_control * global_servo_control;
#define GLOBAL_SERVO_CONTROL global_servo_control

static esp_err_t input_handler (httpd_req_t *req) {
    char data[req->content_len];
    int err = httpd_req_recv(req, data, req->content_len);
    if (err <= 0) {
        Serial.printf("GET read error code: %d\n", err);
        return ESP_FAIL;
    }

    switch (data[0]) {
        case 'u':
            GLOBAL_SERVO_CONTROL->y_servo_add(step);
        break;
        case 'd':
            GLOBAL_SERVO_CONTROL->y_servo_sub(step);
        break;
        case 'r':
            GLOBAL_SERVO_CONTROL->x_servo_add(step);
        break;
        case 'l':
            GLOBAL_SERVO_CONTROL->x_servo_sub(step);
        break;
    }

    httpd_resp_send(req, data, strlen(data));
    return ESP_OK;
}

esp_err_t start_input_handler(httpd_handle_t input_httpd) {

  httpd_uri_t index_uri = {
    .uri       = "/rotate",
    .method    = HTTP_GET,
    .handler   = input_handler,
    .user_ctx  = NULL
  };

  if (GLOBAL_SERVO_CONTROL == NULL) {
    Serial.println("Servo controller not declared");
    return ESP_ERR_TIMEOUT;
  }

  httpd_register_uri_handler(input_httpd, &index_uri);

  return ESP_OK;

}    

void SetServoControl(monitoring_camera_servo_control * servo_control) {
    GLOBAL_SERVO_CONTROL = servo_control;
}


#endif // __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___