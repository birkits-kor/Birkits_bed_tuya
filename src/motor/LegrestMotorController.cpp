#include "LegrestMotorController.h"

LegrestMotorController *LegrestMotorController::instance = nullptr;
MotorState LegrestMotorController::_state = MOTOR_STOPPED;
volatile uint16_t LegrestMotorController::_position = 0;
uint16_t LegrestMotorController::_targetPosition = UINT16_MAX;
uint16_t LegrestMotorController::_travelTime = 0;
unsigned long LegrestMotorController::_isrTime = 0;
String LegrestMotorController::_name = "leg_pos";

LegrestMotorController *LegrestMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new LegrestMotorController();
    return instance;
}

void LegrestMotorController::legrestISR()
{
    _isrTime = millis();
}

void LegrestMotorController::setupMotor()
{
    begin();
    attachInterrupt(digitalPinToInterrupt(_hallPin), legrestISR, RISING);
    String val = NVSStorage::getInstance().getCredential(_name);

    if (val.length() == 0)
    {
        _position = 0;
        NVSStorage::getInstance().saveCredential(_name, "0");
    }
    else
        _position = static_cast<uint16_t>(val.toInt());
    Serial.printf("LegrestMotor set[%d]!!\n", _position);
}

void LegrestMotorController::moveTo(uint16_t targetPosition)
{
     _targetPosition = targetPosition;
    _travelTime = abs(_position - _targetPosition);
    if (_targetPosition == 0 || _targetPosition == LEGREST_MAX)
        _travelTime = LEGREST_MAX;

    if (_position < _targetPosition || _targetPosition >= LEGREST_MAX)
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

void LegrestMotorController::updatePos()
{
    if (_state == MOTOR_STOPPED)
        return;
    if (millis() - _startTime >= _travelTime || millis() - _isrTime > 100) // 정해진 시간만큼 작동 함 or isr 최종 작동 시간 300ms 초과
    {
        if (millis() - _isrTime < 100 && (_targetPosition == 0 || _targetPosition == LEGREST_MAX)) // 끝점 보정 작업 위해 추가 5초 이동
        {
            _startTime = millis();
            _travelTime = 5000;
            return;
        }
        _position = _targetPosition;
        stop();
        Serial.printf("LegrestMotor move done[%d]!!\n", _position);
    }
}

int LegrestMotorController::getPosition()
{
    return map(0, 80, 0, LEGREST_MAX, _position);
}

// void LegrestMotorController::moveUp(uint16_t n)
// {
//     if (!_enabled)
//         return;
//     if (_position + n >= LEGREST_MAX)
//         _targetPosition = LEGREST_MAX;
//     else
//         _targetPosition = _position + n;
//     _startTime = millis() - (20 * 1000);
//     moveForward();
// }

// void LegrestMotorController::moveDown(uint16_t n)
// {
//     if (!_enabled)
//         return;
//     if (_position <= n)
//         _targetPosition = 0;
//     else
//         _targetPosition = _position - n;
//     _startTime = millis() - (20 * 1000);
//     moveBackward();
// }

void LegrestMotorController::moveForward()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
    _state = MOTOR_MOVING_FORWARD;
}

void LegrestMotorController::moveBackward()
{
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_MOVING_BACKWARD;
}

void LegrestMotorController::stopMotor()
{
    if (_state == MOTOR_STOPPED)
        return;
    if (_state == MOTOR_MOVING_FORWARD && _position + (millis() - _startTime) >= LEGREST_MAX)
        _position = LEGREST_MAX;
    else if (_state == MOTOR_MOVING_BACKWARD && _position + (millis() - _startTime) <= 0)
        _position = 0;
    else if (_state == MOTOR_MOVING_FORWARD)
        _position += (millis() - _startTime);
    else if (_state == MOTOR_MOVING_BACKWARD)
        _position -= (millis() - _startTime);
    stop();
    Serial.printf("LegrestMotor move stop[%d]!!\n", _position);
}

void LegrestMotorController::stop()
{
    _state = MOTOR_STOPPED;
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    NVSStorage::getInstance().saveCredential(_name, String((_position / 100) * 100));
}