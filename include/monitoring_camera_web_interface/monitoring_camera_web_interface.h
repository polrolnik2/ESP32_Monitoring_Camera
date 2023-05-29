#ifndef __MONITORING_CAMERA_WEB_INTERFACE_H___
#define __MONITORING_CAMERA_WEB_INTERFACE_H___

#include "monitoring_camera_servo_control/monitoring_camera_servo_control.h"
#include "monitoring_camera_camera_interface/monitoring_camera_camera_interface.h"
#include "monitoring_camera_web_interface/monitoring_camera_web_interface_video_stream_handler.h"
#include "monitoring_camera_web_interface/monitoring_camera_web_interface_input_handler.h"
#include <WiFi.h>
#include <esp_http_server.h>

WiFiServer server(80);

const String site_body ="\
<!DOCTYPE html>\
<html lang=\"en\">\
\
<style>\
    body {\
        background-color: whitesmoke;\
        display: flex;\
        justify-content: center;\
        align-items: center;\
    }\
    \
    .content {\
        width: 474px;\
    }\
    \
    h1 {\
        font-family: -apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,\"Helvetica Neue\",Arial,sans-serif,\"Apple Color Emoji\",\"Segoe UI Emoji\",\"Segoe UI Symbol\";\
        font-size: 42px;\
        text-align: center;\
        color: rgb(27, 27, 27);\
    }\
    \
    #stream {\
        border-radius:5px;\
    }\
    \
    .navigator {\
        margin-top: 50px;\
        margin-left: auto;\
        margin-right: auto;\
        width: 200px;\
        height: 200px;\
        border-radius: 50%;\
        background-color: rgb(196, 196, 196);\
    }\
    \
    .bar {\
        width: 200px;\
        height: 50px;\
        display: flex;\
        justify-content: center;\
        align-items: end;\
    }\
    .bar1 {\
        width: 80px;\
        margin-left: 20px;\
        height: 100px;\
        float: left;\
        display: flex;\
        justify-content: start;\
        align-items: center;\
    }\
    .bar2 {\
        width: 80px;\
        height: 100px;\
        float: left;\
        display: flex;\
        justify-content: end;\
        align-items: center;\
    }\
    .bar3 {\
        width: 200px;\
        height: 50px;\
        float: left;\
        display: flex;\
        justify-content: center;\
        align-items: start;\
    }\
    \
    .button {\
        background-color: rgb(196, 196, 196);\
        border-style: none;\
        font-size: 30px;\
        width: 30px;\
        height: 30px;\
        transition: 0.3s;\
    }\
    \
    .button:hover {\
        transition: 0.3s;\
        color:rgb(122, 122, 122);\
        border-style: none;\
        font-size: 30px;\
        width: 30px;\
        height: 30px;\
    }\
</style>\
\
<head>\
    <meta charset=\"UTF-8\" />\
    <title>Monitoring Camera</title>\
    <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />\
    <meta name=\"description\" content=\"\" />\
    <link rel=\"stylesheet\" href=\"index.css\">\
\
    <script type=\"module\" src=\"https://unpkg.com/ionicons@7.1.0/dist/ionicons/ionicons.esm.js\"></script>\
    <script nomodule src=\"https://unpkg.com/ionicons@7.1.0/dist/ionicons/ionicons.js\"></script>\
</head>\
\
<body> \
    <div class=\"content\">\
        <h1>Monitoring Camera</h1>\
        <img id='stream' src='/stream'>\
\
        <div class=\"navigator\">\
            <div class=\"bar\">\
                <button id=\"u\" onmousedown=\"rotate('u')\" class=\"button\"><ion-icon name='chevron-up-outline'></ion-icon></button>\
            </div>\
            <div class=\"bar1\">\
                <button id=\"l\" onmousedown=\"rotate('l')\" class=\"button\" ><ion-icon name='chevron-back-outline'></ion-icon></button>\
            </div>\
            <div class=\"bar2\">\
                <button id=\"r\" onmousedown=\"rotate('r')\" class=\"button\"><ion-icon name='chevron-forward-outline'></ion-icon></button>\
            </div>\
            <div class=\"bar3\">\
                <button id=\"d\" onmousedown=\"rotate('d')\" class=\"button\" ><ion-icon name='chevron-down-outline'></ion-icon></button>\
            </div>\
        </div>\
    </div>\
\
    <script>\
        function rotate(direction) {\
            console.log(\"rotate function called\");\
            var http = new XMLHttpRequest();\
            var url = \"/rotate?dir='\" + direction + \"'\";\
            http.open(\"GET\", url, true);\
            http.send();\
        }\
    </script>\
\
</body>";

// Main website HTML, JS and CSS code
esp_err_t root_handler(httpd_req_t *req) {

    httpd_resp_set_type(req, "text/html");

    int err = httpd_resp_sendstr(req, site_body.c_str());
    return err;
}

// Main method for initializing the system. Activates stream and input handlers, WiFi and main HTTP server.
esp_err_t web_server_init (const char * SSID, const char * PSK) {
    
    WiFi.begin(SSID, PSK);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    
    Serial.print("Monitoring Camera Stream at: http://");
    Serial.println(WiFi.localIP());

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t stream_httpd = NULL;
    config.server_port = 81;

    httpd_uri_t index_uri = {
      .uri       = "/",
      .method    = HTTP_GET,
      .handler   = root_handler,
      .user_ctx  = NULL
    };
    
    //Serial.printf("Starting web server on port: '%d'\n", config.server_port);

    esp_err_t err = httpd_start(&stream_httpd, &config);
    startCameraServer(stream_httpd);
    err = start_input_handler(stream_httpd);

    if (err != ESP_OK) {
        Serial.println("Error in starting server subhandlers");
        return err;
    }

    if (err == ESP_OK) {
      httpd_register_uri_handler(stream_httpd, &index_uri);
    }

    server.begin();

    Serial.println("Server started");

    return err;
}

#endif // __MONITORING_CAMERA_WEB_INTERFACE_H___