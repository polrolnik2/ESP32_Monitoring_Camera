#ifndef __MONITORING_CAMERA_SERVO_CONTROL_H___
#define __MONITORING_CAMERA_SERVO_CONTROL_H___

#include <ESP32Servo.h>

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

    void x_servo_add(int step);
    void y_servo_add(int step);
    void x_servo_sub(int step);
    void y_servo_sub(int step);
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
}

monitoring_camera_servo_control::~monitoring_camera_servo_control()
{
}

inline void monitoring_camera_servo_control::x_servo_add(int step) {
    X_Servo.write(X_Servo.read() + step);
}

inline void monitoring_camera_servo_control::y_servo_add(int step) {
    Y_Servo.write(Y_Servo.read() + step);
}

inline void monitoring_camera_servo_control::x_servo_sub(int step) {
    X_Servo.write(X_Servo.read() - step);
}

inline void monitoring_camera_servo_control::y_servo_sub(int step) {
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