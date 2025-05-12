#include "TableMotorController.h"

TableMotorController *TableMotorController::instance = nullptr;
volatile bool TableMotorController::_ischange = false;
volatile int TableMotorController::_pulseCount = 0;
bool TableMotorController::_enabled = false;
MotorState TableMotorController::_state = MOTOR_STOPPED;
volatile uint16_t TableMotorController::_position = 0;
uint16_t TableMotorController::_targetPosition = UINT16_MAX;
String TableMotorController::_name = "table_pos";

TableMotorController *TableMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new TableMotorController();
    return instance;
}

void TableMotorController::tableISR()
{
    if (!_enabled)
        _pulseCount++;

    if (_state == MOTOR_MOVING_FORWARD && _position < TABLE_MAX)
        _position++; // 전진 중일 때 포지션 증가
    else if (_state == MOTOR_MOVING_BACKWARD && _position > 0)
        _position--; // 후진 중일 때 포지션 감소

    if (_enabled && (_position == 0 || _position == TABLE_MAX || _position == _targetPosition))
    {
        _ischange = true;
    }
}

void TableMotorController::setupMotor()
{
    begin();
    _enabled = false;
    attachInterrupt(digitalPinToInterrupt(_hallPin), tableISR, RISING);
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
        Serial.printf("TableMotor enable pos[%d]!!\n", _position);
        return;
    }
    else
        Serial.printf("TableMotor disable!!\n");
}

void TableMotorController::moveTo(uint16_t targetPosition)
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

void TableMotorController::updatePos()
{
    if (!_enabled)
        return;

    if (_ischange)
    {
        _ischange = false;
        Serial.printf("TableMotor move done!!\n");
        if (_position == 0 || _position == TABLE_MAX)
            delay(1000);
        TableMotorController::getInstance()->stopMotor();
        NVSStorage::getInstance().saveCredential(_name, String((_position / 10) * 10));
    }

    // 끝점 값 추가 보정
    if (_state != MOTOR_STOPPED && millis() > _startTime + 30 * 1000)
    {
        _ischange = true;
        if (_state == MOTOR_MOVING_FORWARD)
            _position = TABLE_MAX;
        else if (_state == MOTOR_MOVING_BACKWARD)
            _position = 0;
    }
}

int TableMotorController::getPosition()
{
    return map(0, 80, 0, TABLE_MAX, _position);
}

void TableMotorController::moveUp(uint16_t n)
{
    if (!_enabled)
        return;
    if (_position + n >= TABLE_MAX)
        _targetPosition = TABLE_MAX;
    else
        _targetPosition = _position + n;
    _startTime = millis() - (20 * 1000);
    moveForward();
}

void TableMotorController::moveDown(uint16_t n)
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

void TableMotorController::moveForward()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
    _state = MOTOR_MOVING_FORWARD;
}

void TableMotorController::moveBackward()
{
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_MOVING_BACKWARD;
}

void TableMotorController::stopMotor()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    _state = MOTOR_STOPPED;
}
