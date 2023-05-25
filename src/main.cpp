#include <Arduino.h>
#include <Monitoring_Camera.h>

monitoring_camera_servo_control ServoControl(_x_servo_pin, _y_servo_pin);

void setup() {
    Serial.begin(115200);
    SetServoControl(&ServoControl);
    web_server_init("Kaplica", "Krzysiek2012");
}

void loop() {
  // put your main code here, to run repeatedly:
}