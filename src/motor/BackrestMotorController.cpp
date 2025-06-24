#include "BackrestMotorController.h"

BackrestMotorController *BackrestMotorController::instance = nullptr;
MotorState BackrestMotorController::_state = MOTOR_STOPPED;
volatile uint16_t BackrestMotorController::_position = 0;
uint16_t BackrestMotorController::_targetPosition = UINT16_MAX;
uint16_t BackrestMotorController::_travelTime = 0;
unsigned long BackrestMotorController::_isrTime = 0;
String BackrestMotorController::_name = "back_pos";

BackrestMotorController *BackrestMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new BackrestMotorController();
    return instance;
}

void BackrestMotorController::backrestISR()
{
    _isrTime = millis();
}

void BackrestMotorController::setupMotor()
{
    begin();
    attachInterrupt(digitalPinToInterrupt(_hallPin), backrestISR, RISING);
    String val = NVSStorage::getInstance().getCredential(_name);

    if (val.length() == 0)
    {
        _position = 0;
        NVSStorage::getInstance().saveCredential(_name, "0");
    }
    else
        _position = static_cast<uint16_t>(val.toInt());
    Serial.printf("BackrestMotor set[%d]!!\n", _position);
}

void BackrestMotorController::moveTo(uint16_t targetPosition)
{
    _targetPosition = targetPosition;
    if(_targetPosition == _position)
        return;
    _travelTime = abs(_position - _targetPosition);
    if (_targetPosition == 0 || _targetPosition == BACKREST_MAX)
        _travelTime = BACKREST_MAX;

    if (_position < _targetPosition || _targetPosition >= BACKREST_MAX)
    {
        _startTime = millis();
        _isrTime = millis();
        moveForward();
    }
    else if (_position > _targetPosition || _targetPosition == 0)
    {
        _startTime = millis();
        _isrTime = millis();
        moveBackward();
    }
    else
        stopMotor();
}

void BackrestMotorController::updatePos()
{
    if (_state == MOTOR_STOPPED)
        return;
    if (millis() - _startTime >= _travelTime || millis() - _isrTime > 50) // 정해진 시간만큼 작동 함 or isr 최종 작동 시간 300ms 초과
    {
        if (millis() - _isrTime < 50 && (_targetPosition == 0 || _targetPosition == BACKREST_MAX)) // 끝점 보정 작업 위해 추가 5초 이동
        {
            _startTime = millis();
            _travelTime = 5000;
            return;
        }
        _position = _targetPosition;
        stop();
        Serial.printf("BackrestMotor move done[%d]!!\n", _position);
    }
}

int BackrestMotorController::getPosition()
{
    return map(0, 80, 0, BACKREST_MAX, _position);
}

// void BackrestMotorController::moveUp(uint16_t n)
// {
//     if (!_enabled)
//         return;
//     if (_position + n >= BACKREST_MAX)
//         _targetPosition = BACKREST_MAX;
//     else
//         _targetPosition = _position + n;
//     _startTime = millis() - (20 * 1000); // 최대 10초 이동동
//     moveForward();
// }

// void BackrestMotorController::moveDown(uint16_t n)
// {
//     if (!_enabled)
//         return;
//     if (_position <= n)
//         _targetPosition = 0;
//     else
//         _targetPosition = _position - n;
//     _startTime = millis() - (20 * 1000); // 최대 10초 이동동
//     moveBackward();
// }

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
    if (_state == MOTOR_STOPPED)
        return;
    if (_state == MOTOR_MOVING_FORWARD && _position + (millis() - _startTime) >= BACKREST_MAX)
        _position = BACKREST_MAX;
    else if (_state == MOTOR_MOVING_BACKWARD && _position + (millis() - _startTime) <= 0)
        _position = 0;
    else if (_state == MOTOR_MOVING_FORWARD)
        _position += (millis() - _startTime);
    else if (_state == MOTOR_MOVING_BACKWARD)
        _position -= (millis() - _startTime);
    stop();
    Serial.printf("BackrestMotor move stop[%d]!!\n", _position);
}

void BackrestMotorController::stop()
{
    _state = MOTOR_STOPPED;
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    NVSStorage::getInstance().saveCredential(_name, String((_position / 100) * 100));
}