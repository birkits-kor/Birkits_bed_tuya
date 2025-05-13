#ifndef MOTOR_ROUTINE_H
#define MOTOR_ROUTINE_H

#include <Arduino.h>
#include <map>
#include "../motor/BackrestMotorController.h"
#include "../motor/LegrestMotorController.h"
#include "../motor/TableMotorController.h"
#include "../motor/SpeakerController.h"

class MotorRoutine
{
public:
    MotorRoutine();
    void begin();
    void updatePos();


    void addMotor(const String &motorName, MotorController *motor);
    void moveForward(const String &motorName);
    void moveBackward(const String &motorName);
    void stopMotor(const String &motorName);
    int getSensorValue(const String &motorName);
    void stopAllMotors();
    

private:

};

#endif
