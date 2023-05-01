#include <ESP32Servo.h>
#include <unity.h>
#include "monitoring_camera_servo_control/monitoring_camera_servo_control.h"

#define x_servo_pin 2
#define y_servo_pin 3
#define step 1
#define HertzPeriod 50

monitoring_camera_servo_control servo_control(x_servo_pin, y_servo_pin, HertzPeriod);

void test_check_servo_attached(void) {
    TEST_ASSERT_EQUAL(1, servo_control.servo_attached());
}

void test_servo_set_0(void) {
    servo_control.x_servo_set(0);
    servo_control.y_servo_set(0);
    TEST_ASSERT_EQUAL(0, servo_control.x_servo_read());
    TEST_ASSERT_EQUAL(0, servo_control.y_servo_read());
    delay(1000);
}

void test_servo_x_add_180(void) {
    int servo_expected_pos = servo_control.x_servo_read();
    for (int i = 0; i < int(180/step); i++) {
        servo_control.x_servo_add(step);
        TEST_ASSERT_EQUAL(servo_expected_pos + step, servo_control.x_servo_read());
        servo_expected_pos + step;
        delay(1000/(180/step));
    }
    TEST_ASSERT_EQUAL(servo_expected_pos, servo_control.x_servo_read());
}

void test_servo_y_add_45(void) {
    int servo_expected_pos = servo_control.y_servo_read();
    for (int i = 0; i < int(45/step); i++) {
        servo_control.y_servo_add(step);
        TEST_ASSERT_EQUAL(servo_expected_pos + step, servo_control.y_servo_read());
        servo_expected_pos + step;
        delay(1000/(45/step));
    }
    TEST_ASSERT_EQUAL(servo_expected_pos, servo_control.y_servo_read());
}

void test_servo_x_sub_90(void) {
    int servo_expected_pos = servo_control.x_servo_read();
    for (int i = 0; i < int(90/step); i++) {
        servo_control.x_servo_sub(step);
        TEST_ASSERT_EQUAL(servo_expected_pos - step, servo_control.x_servo_read());
        servo_expected_pos - step;
        delay(1000/(90/step));
    }
    TEST_ASSERT_EQUAL(servo_expected_pos, servo_control.x_servo_read());
}

void test_servo_y_sub_45(void) {
    int servo_expected_pos = servo_control.y_servo_read();
    for (int i = 0; i < int(45/step); i++) {
        servo_control.y_servo_sub(step);
        TEST_ASSERT_EQUAL(servo_expected_pos - step, servo_control.y_servo_read());
        servo_expected_pos - step;
        delay(1000/(45/step));
    }
    TEST_ASSERT_EQUAL(servo_expected_pos, servo_control.y_servo_read());
}

int run_test(void) {
    UNITY_BEGIN();

    RUN_TEST(test_check_servo_attached);
    RUN_TEST(test_servo_set_0);
    RUN_TEST(test_servo_x_add_180);
    RUN_TEST(test_servo_y_add_45);
    RUN_TEST(test_servo_x_sub_90);
    RUN_TEST(test_servo_y_sub_45);

    return UNITY_END();
}

void setup() {
    delay(2000);

    run_test();
}

void loop() {}