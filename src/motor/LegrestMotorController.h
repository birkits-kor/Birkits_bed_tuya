#ifndef LEGREST_MOTOR_CONTROLLER_H
#define LEGREST_MOTOR_CONTROLLER_H

#include "MotorController.h"

// LEGREST 모터 핀 상수 정의
#define LEGREST_MOTOR_PIN1 22
#define LEGREST_MOTOR_PIN2 23
#define LEGREST_HALL_SENSOR_PIN 26
#define LEGREST_MAX 15000

class LegrestMotorController : public MotorController
{
public:
    static LegrestMotorController *getInstance();

    void setupMotor();
    void moveTo(uint16_t targetPosition);
    void updatePos();
    int getPosition();
    // void moveUp(uint16_t n);
    // void moveDown(uint16_t n);
    void stopMotor();


private:
    LegrestMotorController()
        : MotorController(LEGREST_MOTOR_PIN1, LEGREST_MOTOR_PIN2, LEGREST_HALL_SENSOR_PIN) {}

    static LegrestMotorController *instance;
    static void legrestISR();
    void moveForward();
    void moveBackward();
    void stop();

    static MotorState _state;
    static volatile uint16_t _position;
    static uint16_t _targetPosition;
    static uint16_t _travelTime;
    static unsigned long _isrTime;
    static String _name;

    unsigned long _startTime = 0;
};

#endif