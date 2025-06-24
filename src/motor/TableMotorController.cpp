#include "TableMotorController.h"

TableMotorController *TableMotorController::instance = nullptr;
MotorState TableMotorController::_state = MOTOR_STOPPED;
volatile uint16_t TableMotorController::_position = 0;
uint16_t TableMotorController::_targetPosition = UINT16_MAX;
uint16_t TableMotorController::_travelTime = 0;
unsigned long TableMotorController::_isrTime = 0;
String TableMotorController::_name = "table_pos";

TableMotorController *TableMotorController::getInstance()
{
    if (instance == nullptr)
        instance = new TableMotorController();
    return instance;
}

void TableMotorController::tableISR()
{
    _isrTime = millis();
}

void TableMotorController::setupMotor()
{
    begin();
    attachInterrupt(digitalPinToInterrupt(_hallPin), tableISR, RISING);
    String val = NVSStorage::getInstance().getCredential(_name);

    if (val.length() == 0)
    {
        _position = 0;
        NVSStorage::getInstance().saveCredential(_name, "0");
    }
    else
        _position = static_cast<uint16_t>(val.toInt());
    Serial.printf("TableMotor set[%d]!!\n", _position);
}

void TableMotorController::moveTo(uint16_t targetPosition)
{
    _targetPosition = targetPosition;
    _travelTime = abs(_position - _targetPosition);
    if (_targetPosition == 0 || _targetPosition == TABLE_MAX)
        _travelTime = TABLE_MAX;

    if (_position < _targetPosition || _targetPosition >= TABLE_MAX)
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

void TableMotorController::updatePos()
{
    if (_state == MOTOR_STOPPED)
        return;
    if (millis() - _startTime >= _travelTime || millis() - _isrTime > 50) // 정해진 시간만큼 작동 함 or isr 최종 작동 시간 300ms 초과
    {
        if (millis() - _isrTime < 50 && (_targetPosition == 0 || _targetPosition == TABLE_MAX)) // 끝점 보정 작업 위해 추가 5초 이동
        {
            _startTime = millis();
            _travelTime = 5000;
            return;
        }
        _position = _targetPosition;
        stop();
        Serial.printf("TableMotor move done[%d]!!\n", _position);
    }
}

int TableMotorController::getPosition()
{
    return map(0, 80, 0, TABLE_MAX, _position);
}

// void TableMotorController::moveUp(uint16_t n)
// {
//     if (!_enabled)
//         return;
//     if (_position + n >= TABLE_MAX)
//         _targetPosition = TABLE_MAX;
//     else
//         _targetPosition = _position + n;
//     _startTime = millis() - (20 * 1000);
//     moveForward();
// }

// void TableMotorController::moveDown(uint16_t n)
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
    if (_state == MOTOR_STOPPED)
        return;
    if (_state == MOTOR_MOVING_FORWARD && _position + (millis() - _startTime) >= TABLE_MAX)
        _position = TABLE_MAX;
    else if (_state == MOTOR_MOVING_BACKWARD && _position + (millis() - _startTime) <= 0)
        _position = 0;
    else if (_state == MOTOR_MOVING_FORWARD)
        _position += (millis() - _startTime);
    else if (_state == MOTOR_MOVING_BACKWARD)
        _position -= (millis() - _startTime);
    stop();
    Serial.printf("TableMotor move stop[%d]!!\n", _position);
}

void TableMotorController::stop()
{
    _state = MOTOR_STOPPED;
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    NVSStorage::getInstance().saveCredential(_name, String((_position / 100) * 100));
}