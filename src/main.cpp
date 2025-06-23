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
    mainRoutine.do50msTasks();
    mainRoutine.do100msTasks();
    mainRoutine.do1mTasks();
    delay(1);
}