#include "BackrestMotorController.h"

BackrestMotorController *BackrestMotorController::instance = nullptr;
volatile int BackrestMotorController::_pulseCount = 0;
bool BackrestMotorController::_enabled = false;
MotorState BackrestMotorController::_state = MOTOR_STOPPED;
volatile uint16_t BackrestMotorController::_position = 0;
uint16_t BackrestMotorController::_targetPosition = UINT16_MAX;
String BackrestMotorController::_name = "back_pos";

BackrestMotorController *BackrestMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new BackrestMotorController();
    return instance;
}

void BackrestMotorController::backrestISR()
{
    if (!_enabled)
        _pulseCount++;

    if (_state == MOTOR_MOVING_FORWARD && _position < BACKREST_MAX)
        _position++; // 전진 중일 때 포지션 증가
    else if (_state == MOTOR_MOVING_BACKWARD && _position > 0)
        _position--; // 후진 중일 때 포지션 감소
    if (_enabled && (_position == 0 || _position == BACKREST_MAX || _position == _targetPosition))
    {
        delay(1000);
        BackrestMotorController::getInstance()->stopMotor();
        NVSStorage::getInstance().saveCredential(_name, String(_position));
    }
}

void BackrestMotorController::setupMotor()
{
    begin();
    _enabled = false;
    attachInterrupt(digitalPinToInterrupt(_hallPin), backrestISR, RISING);
    String val = NVSStorage::getInstance().getCredential(_name);

    if (val.length() == 0)
    {
        _position = 0;
        NVSStorage::getInstance().saveCredential(_name, "0");
    }

    _pulseCount = 0;
    moveForward();
    delay(1000); // 0.1초 전진
    stopMotor();
    delay(100);

    moveBackward();
    delay(1000); // 0.1초 후진
    stopMotor();

    if (_pulseCount != 0)
    {
        _enabled = true;
        Serial.printf("BackrestMotor enable pos[%d]!!", _position);
    }
}

void BackrestMotorController::moveTo(uint16_t targetPosition)
{
    _targetPosition = targetPosition;
    if (!_enabled)
        return;
    if (_position < _targetPosition)
        moveForward();
    else if (_position > _targetPosition)
        moveBackward();
    else
        stopMotor();
}

int BackrestMotorController::getPosition()
{
    return _position;
}

MotorState BackrestMotorController::getState()
{
    return _state;
}

void BackrestMotorController::moveForward()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
    _state = MOTOR_MOVING_FORWARD;
}

void BackrestMotorController::moveBackward()
{
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_MOVING_BACKWARD;
}

void BackrestMotorController::stopMotor()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_STOPPED;
}