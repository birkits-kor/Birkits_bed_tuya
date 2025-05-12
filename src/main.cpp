#include <Arduino.h>
#include "MainRoutine.h"

MainRoutine mainRoutine;

void setup()
{
    mainRoutine.init();
}

void loop()
{
    mainRoutine.do1msTasks();
    mainRoutine.do10msTasks();
    mainRoutine.do100msTasks();

    if (Serial.available() > 0)
    {
        auto receivedChar = Serial.read(); // 한 글자 읽기
        if (receivedChar == 'q')
        {
            BackrestMotorController::getInstance()->moveUp(50);
        }
        if (receivedChar == 'a')
        {
            BackrestMotorController::getInstance()->moveDown(50);
        }
    }
}


// #define BACKREST_MOTOR_PIN1 18
// #define BACKREST_MOTOR_PIN2 19
// #define LEGS_MOTOR_PIN1 22
// #define LEGS_MOTOR_PIN2 23
// #define TABLE_MOTOR_PIN1 12
// #define TABLE_MOTOR_PIN2 13
// #define BACKREST_HALL_SENSOR_PIN 25
// #define LEGS_HALL_SENSOR_PIN 26
// #define TABLE_HALL_SENSOR_PIN 27