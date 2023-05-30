#ifndef __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___
#define __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___

#include <WiFi.h>
#include <esp_http_server.h>
#include <monitoring_camera_servo_control/monitoring_camera_servo_control.h>

/*-------------------------------------------------------------------------------------------
Methods for providing communication between the HTTP website and the Servo controller class.
Reads the GET code of the site request and activates the appropriate servo control function.
-------------------------------------------------------------------------------------------*/

monitoring_camera_servo_control * global_servo_control;
#define GLOBAL_SERVO_CONTROL global_servo_control

// Method for controlling site input requests
static esp_err_t input_handler (httpd_req_t *req) {
    Serial.println("Input request detected");
    String uri = req->uri;

    switch (uri[uri.indexOf("=") + 1]) {
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

    httpd_resp_send(req, uri.c_str(), strlen(uri.c_str()));
    return ESP_OK;
}

// Starts URI handler for the inputs
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

// Associates a servo object with the site handler
void SetServoControl(monitoring_camera_servo_control * servo_control) {
    GLOBAL_SERVO_CONTROL = servo_control;
}


#endif // __MONITORING_CAMERA_WEB_INTERFACE_INPUT_HANDLER_H___