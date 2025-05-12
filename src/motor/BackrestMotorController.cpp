#include "BackrestMotorController.h"

BackrestMotorController *BackrestMotorController::instance = nullptr;
volatile bool BackrestMotorController::_ischange = false;
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
        _ischange = true;
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
    delay(100); // 0.1초 전진
    stopMotor();
    delay(10);

    moveBackward();
    delay(100); // 0.1초 후진
    stopMotor();

    if (_pulseCount != 0)
    {
        _enabled = true;
        Serial.printf("BackrestMotor enable pos[%d]!!\n", _position);
        return;
    }
    else
        Serial.printf("BackrestMotor disable!!\n");
}

void BackrestMotorController::moveTo(uint16_t targetPosition)
{
    if (!_enabled)
        return;
    _targetPosition = targetPosition;
    if (_position < _targetPosition)
    {
        _startTime = millis();
        moveForward();
    }
    else if (_position > _targetPosition)
    {
        _startTime = millis();
        moveBackward();
    }
    else
        stopMotor();
}

void BackrestMotorController::updatePos()
{
    if (!_enabled)
        return;
    
    if (_ischange)
    {
        _ischange = false;
        Serial.printf("BackrestMotor move done!!\n");
        if (_position == 0 || _position == BACKREST_MAX)
            delay(1000);
        BackrestMotorController::getInstance()->stopMotor();
        NVSStorage::getInstance().saveCredential(_name, String((_position / 10) * 10));
    }

    // 끝점 값 추가 보정
    if (_state != MOTOR_STOPPED && millis() > _startTime + 30 * 1000)
    {
        _ischange = true;
        if(_state == MOTOR_MOVING_FORWARD)          
            _position = BACKREST_MAX;
        else if(_state == MOTOR_MOVING_BACKWARD)
            _position = 0;
    }
}

int BackrestMotorController::getPosition()
{
    return map(0, 80, 0, BACKREST_MAX, _position);
}

void BackrestMotorController::moveUp(uint16_t n)
{
    if (_position + n >= BACKREST_MAX)
        _targetPosition = BACKREST_MAX;
    else
        _targetPosition = _position + n;
    _startTime = millis() - (20 * 1000); // 최대 10초 이동동
    moveForward();
}

void BackrestMotorController::moveDown(uint16_t n)
{
    if (_position <= n)
        _targetPosition = 0;
    else
        _targetPosition = _position - n;
    _startTime = millis() - (20 * 1000); // 최대 10초 이동동
    moveBackward();
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