#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>

class MotorController
{
public:
    MotorController(int pin1, int pin2, int hallPin)
        : _pin1(pin1), _pin2(pin2), _hallPin(hallPin) {}

    void begin()
    {
        pinMode(_pin1, OUTPUT);
        pinMode(_pin2, OUTPUT);
        pinMode(_hallPin, INPUT);
        stopMotor(); // 초기 상태는 정지
    }

    void moveForward()
    {
        digitalWrite(_pin1, LOW);
        digitalWrite(_pin2, HIGH);        
    }

    void moveBackward()
    {
        digitalWrite(_pin1, HIGH);
        digitalWrite(_pin2, LOW);
    }

    void stopMotor()
    {
        digitalWrite(_pin1, LOW);
        digitalWrite(_pin2, LOW);
    }

    int getSensorValue()
    {
        return digitalRead(_hallPin);
    }

private:
    int _pin1;
    int _pin2;
    int _hallPin;
};

#endif
