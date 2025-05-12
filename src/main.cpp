#include <Arduino.h>
#include "MainRoutine.h"

MainRoutine mainRoutine;
void keyboardTest(char c);

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
        keyboardTest(Serial.read());
}

void keyboardTest(char c)
{
    if (c == 'q')
        BackrestMotorController::getInstance()->moveUp(50);
    if (c == 'a')
        BackrestMotorController::getInstance()->moveDown(50);
    if (c == 'w')
        LegrestMotorController::getInstance()->moveUp(50);
    if (c == 's')
        LegrestMotorController::getInstance()->moveDown(50);
    if (c == 'e')
        TableMotorController::getInstance()->moveUp(50);
    if (c == 'd')
        TableMotorController::getInstance()->moveDown(50);
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