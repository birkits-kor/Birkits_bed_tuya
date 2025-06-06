#include <Arduino.h>
#include "MainRoutine.h"

MainRoutine mainRoutine;
void keyboardTest(char c);

void setup()
{
    mainRoutine.init();
    pinMode(34, INPUT);
    pinMode(35, INPUT);
}

void loop()
{
    mainRoutine.do1msTasks();
    mainRoutine.do10msTasks();
    mainRoutine.do50msTasks();
    mainRoutine.do100msTasks();
    delay(1);
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
    if (c == 'z')
        SpeakerController::getInstance()->on();
    if (c == 'x')
        SpeakerController::getInstance()->off();
     
}