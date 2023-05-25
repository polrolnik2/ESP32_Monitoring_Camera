#ifndef __MONITORING_CAMERA_WEB_INTERFACE_H___
#define __MONITORING_CAMERA_WEB_INTERFACE_H___

#include "monitoring_camera_servo_control/monitoring_camera_servo_control.h"
#include "monitoring_camera_camera_interface/monitoring_camera_camera_interface.h"
#include "monitoring_camera_web_interface/monitoring_camera_web_interface_video_stream_handler.h"
#include "monitoring_camera_web_interface/monitoring_camera_web_interface_input_handler.h"
#include <WiFi.h>
#include <esp_http_server.h>

WiFiServer server(80);

esp_err_t root_handler(httpd_req_t *req) {

    httpd_resp_set_type(req, "text/html");

    int err = httpd_resp_sendstr(req, \
    "<!DOCTYPE html>\
    <html lang=\"en\">\
    \
    <head>\
        <meta charset=\"UTF-8\" />\
        <title>Monitoring Camera</title>\
        <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\" />\
        <meta name=\"description\" content=\"\" />\
    </head>\
    \
    <body> \
        <h1>Monitoring Camera</h1>\
        <img id='stream' src=''>\
        <input type=\"button\" id=\"u\" value=\"up\"/>\
        <input type=\"button\" id=\"d\" value=\"down\"/>\
        <input type=\"button\" id=\"l\" value=\"left\"/>\
        <input type=\"button\" id=\"r\" value=\"right\"/>\
    \
        <script>\
            function rotate(direction) {\
                var http = new XMLHttpRequest();\
                var url = \"/rotate\";\
                var dir = \"dir=\"+direction;\
                http.open(\"GET\", url, true);\
                http.onreadystatechange = function() {\
                    if(http.readyState == 4 && http.status == 200) {\
                        alert(http.responseText);\
                    }\
                }\
                http.send(dir);\
            }\
            \
            window.onload = document.getElementById(\"stream\").src = window.location.href.slice(0, -1) + \":81/stream\"; \
        \
            document.addEventListener('DOMContentLoaded', function() {\
                document.getElementById(\"u\").addEventListener(\"click\", rotate(\"u\"));\
                document.getElementById(\"d\").addEventListener(\"click\", rotate(\"d\"));\
                document.getElementById(\"l\").addEventListener(\"click\", rotate(\"l\"));\
                document.getElementById(\"r\").addEventListener(\"click\", rotate(\"r\"));\
            })\
        </script>\
    \
    </body>");
    return err;
}

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