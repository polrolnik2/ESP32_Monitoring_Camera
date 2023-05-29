#ifndef __MONITORING_CAMERA_SERVO_CONTROL_H___
#define __MONITORING_CAMERA_SERVO_CONTROL_H___

/*----------------------------------------------------------------
Class structure storing configuration information for the servos.
Cointains various methods of controlling the camera orientation.
----------------------------------------------------------------*/

#include <ESP32Servo.h>

#define step 1

class monitoring_camera_servo_control
{
private:
    int x_servo_pin;
    int y_servo_pin;
    Servo X_Servo;
    Servo Y_Servo;
public:
    monitoring_camera_servo_control(int x_servo_pin, int y_servo_pin, int PeriodHertz);
    ~monitoring_camera_servo_control();

    void x_servo_add(int step_val);
    void y_servo_add(int step_val);
    void x_servo_sub(int step_val);
    void y_servo_sub(int step_val);
    void x_servo_set(int angle);
    void y_servo_set(int angle);

    int servo_attached();
    int y_servo_read();
    int x_servo_read();
};

monitoring_camera_servo_control::monitoring_camera_servo_control(int x_servo_pin, int y_servo_pin, int PeriodHertz = 50)
{
    X_Servo.setPeriodHertz(PeriodHertz);
    X_Servo.attach(x_servo_pin);
    Y_Servo.setPeriodHertz(PeriodHertz);
    Y_Servo.attach(y_servo_pin);
    x_servo_set(0);
    y_servo_set(0);
}

monitoring_camera_servo_control::~monitoring_camera_servo_control()
{
}

inline void monitoring_camera_servo_control::x_servo_add(int step_val) {
    Serial.printf("x_servo_add %d\n", step_val);
    X_Servo.write(X_Servo.read() + step);
}

inline void monitoring_camera_servo_control::y_servo_add(int step_val) {
    Serial.printf("y_servo_add %d\n", step_val);
    Y_Servo.write(Y_Servo.read() + step);
}

inline void monitoring_camera_servo_control::x_servo_sub(int step_val) {
    Serial.printf("x_servo_sub %d\n", step_val);
    X_Servo.write(X_Servo.read() - step);
}

inline void monitoring_camera_servo_control::y_servo_sub(int step_val) {
    Serial.printf("y_servo_sub %d\n", step_val);
    Y_Servo.write(Y_Servo.read() - step);
}

inline void monitoring_camera_servo_control::x_servo_set(int angle) {
    X_Servo.write(angle);
}

inline void monitoring_camera_servo_control::y_servo_set(int angle) {
    Y_Servo.write(angle);
}

inline int monitoring_camera_servo_control::servo_attached() {
    return X_Servo.attached() && Y_Servo.attached();
}

inline int monitoring_camera_servo_control::y_servo_read() {
    return Y_Servo.read();
}

inline int monitoring_camera_servo_control::x_servo_read() {
    return X_Servo.read();
}

#endif // __MONITORING_CAMERA_SERVO_CONTROL_H___