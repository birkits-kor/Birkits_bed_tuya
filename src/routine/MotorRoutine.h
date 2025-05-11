#ifndef MOTOR_ROUTINE_H
#define MOTOR_ROUTINE_H

#include <Arduino.h>
#include <map>
#include "../motor/BackrestMotorController.h"

// 모터 핀 정의
#define LEGS_MOTOR_PIN1 22
#define LEGS_MOTOR_PIN2 23
#define TABLE_MOTOR_PIN1 12
#define TABLE_MOTOR_PIN2 13

// 홀 센서 핀 정의

#define LEGS_HALL_SENSOR_PIN 26
#define TABLE_HALL_SENSOR_PIN 27

class MotorRoutine
{
public:
    MotorRoutine();
    void addMotor(const String &motorName, MotorController *motor);
    void moveForward(const String &motorName);
    void moveBackward(const String &motorName);
    void stopMotor(const String &motorName);
    int getSensorValue(const String &motorName);
    void stopAllMotors();
    void begin();

private:

};

#endif
