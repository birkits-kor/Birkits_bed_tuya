#include "MotorController.h"

MotorController::MotorController(int pin1, int pin2, int hallPin)
    : _pin1(pin1), _pin2(pin2), _hallPin(hallPin)
{
}

void MotorController::begin()
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_hallPin, INPUT);
}


void MotorController::savePosition()
{
    // 위치 값을 저장하는 로직 추가 가능 (예: EEPROM에 저장)
    // 예시: EEPROM.write(0, _position);
}
