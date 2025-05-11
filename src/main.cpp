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
        if (receivedChar == 'a')
        {
            Serial.println('a');
            BackrestMotorController::getInstance()->moveTo(500);
        }
        if (receivedChar == 'b')
        {
            Serial.println('b');
            BackrestMotorController::getInstance()->moveTo(0);
        }
    }
}
