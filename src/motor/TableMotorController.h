#ifndef TABLE_MOTOR_CONTROLLER_H
#define TABLE_MOTOR_CONTROLLER_H

#include "MotorController.h"

// TABLE 모터 핀 상수 정의
#define TABLE_MOTOR_PIN1 12
#define TABLE_MOTOR_PIN2 13
#define TABLE_HALL_SENSOR_PIN 27
#define TABLE_MAX 1384

class TableMotorController : public MotorController
{
public:
    static TableMotorController *getInstance();

    void setupMotor();
    void moveTo(uint16_t targetPosition);
    void updatePos();
    int getPosition();
    void moveUp(uint16_t n);
    void moveDown(uint16_t n);

private:
    TableMotorController()
        : MotorController(TABLE_MOTOR_PIN1, TABLE_MOTOR_PIN2, TABLE_HALL_SENSOR_PIN) {}

    static TableMotorController *instance;
    static void tableISR();
    void moveForward();
    void moveBackward();
    void stopMotor();

    static volatile bool _ischange;
    static bool _enabled;
    static volatile int _pulseCount;
    static MotorState _state;
    static volatile uint16_t _position;
    static uint16_t _targetPosition;
    static String _name;

    unsigned long _startTime = 0;
};

#endif
