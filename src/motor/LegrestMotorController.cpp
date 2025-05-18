#include "LegrestMotorController.h"

LegrestMotorController *LegrestMotorController::instance = nullptr;
volatile bool LegrestMotorController::_ischange = false;
volatile int LegrestMotorController::_pulseCount = 0;
bool LegrestMotorController::_enabled = false;
MotorState LegrestMotorController::_state = MOTOR_STOPPED;
volatile uint16_t LegrestMotorController::_position = 0;
uint16_t LegrestMotorController::_targetPosition = UINT16_MAX;
String LegrestMotorController::_name = "leg_pos";

LegrestMotorController *LegrestMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new LegrestMotorController();
    return instance;
}

void LegrestMotorController::legrestISR()
{
    if (!_enabled)
        _pulseCount++;

    if (_state == MOTOR_MOVING_FORWARD && _position < LEGREST_MAX)
        _position++; // 전진 중일 때 포지션 증가
    else if (_state == MOTOR_MOVING_BACKWARD && _position > 0)
        _position--; // 후진 중일 때 포지션 감소

    if (_enabled && (_position == 0 || _position == LEGREST_MAX || _position == _targetPosition))
    {
        _ischange = true;
    }
}

void LegrestMotorController::setupMotor()
{
    begin();
    _enabled = false;
    attachInterrupt(digitalPinToInterrupt(_hallPin), legrestISR, RISING);
    String val = NVSStorage::getInstance().getCredential(_name);

    if (val.length() == 0)
    {
        _position = 0;
        NVSStorage::getInstance().saveCredential(_name, "0");
    }

    _pulseCount = 0;
    moveForward();
    delay(100); // 0.1초 전진
    stopMotor();
    delay(10);

    moveBackward();
    delay(100); // 0.1초 후진
    stopMotor();

    if (_pulseCount != 0)
    {
        _enabled = true;
        Serial.printf("LegrestMotor enable pos[%d]!!\n", _position);
        return;
    }
    else
        Serial.printf("LegrestMotor disable!!\n");
}

void LegrestMotorController::moveTo(uint16_t targetPosition)
{
    if (!_enabled)
        return;
    _targetPosition = targetPosition;
    if (_position < _targetPosition || _targetPosition >= LEGREST_MAX)
    {
        _startTime = millis();
        moveForward();
    }
    else if (_position > _targetPosition || _targetPosition == 0)
    {
        _startTime = millis();
        moveBackward();
    }
    else
        stopMotor();
}

void LegrestMotorController::updatePos()
{
    if (!_enabled)
        return;

    if (_ischange)
    {
        Serial.printf("LegrestMotor move done[%d]!!\n", _position);
        if (_position == 0 || _position == LEGREST_MAX)
            delay(1000);
        stopMotor();
        _ischange = false;
        NVSStorage::getInstance().saveCredential(_name, String((_position / 10) * 10));
    }

    // 끝점 값 추가 보정
    if (_state != MOTOR_STOPPED && millis() > _startTime + 30 * 1000)
    {
        _ischange = true;
        if (_state == MOTOR_MOVING_FORWARD)
            _position = LEGREST_MAX;
        else if (_state == MOTOR_MOVING_BACKWARD)
            _position = 0;
    }
}

int LegrestMotorController::getPosition()
{
    return map(0, 80, 0, LEGREST_MAX, _position);
}

void LegrestMotorController::moveUp(uint16_t n)
{
    if (!_enabled)
        return;
    if (_position + n >= LEGREST_MAX)
        _targetPosition = LEGREST_MAX;
    else
        _targetPosition = _position + n;
    _startTime = millis() - (20 * 1000);
    moveForward();
}

void LegrestMotorController::moveDown(uint16_t n)
{
    if (!_enabled)
        return;
    if (_position <= n)
        _targetPosition = 0;
    else
        _targetPosition = _position - n;
    _startTime = millis() - (20 * 1000);
    moveBackward();
}

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
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_STOPPED;
    _ischange = true;
}
