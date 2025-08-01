#ifndef BACKREST_MOTOR_CONTROLLER_H
#define BACKREST_MOTOR_CONTROLLER_H

#include "MotorController.h"

// BACKREST 모터 핀 상수 정의
#define BACKREST_MOTOR_PIN1 18
#define BACKREST_MOTOR_PIN2 19
#define BACKREST_HALL_SENSOR_PIN 25
#define BACKREST_MAX 35000

class BackrestMotorController : public MotorController
{
public:
    static BackrestMotorController *getInstance();

    void setupMotor();
    void moveTo(uint16_t targetPosition);
    void updatePos();
    int getPosition();
    // void moveUp(uint16_t n);
    // void moveDown(uint16_t n);
    void stopMotor();

private:
    BackrestMotorController()
        : MotorController(BACKREST_MOTOR_PIN1, BACKREST_MOTOR_PIN2, BACKREST_HALL_SENSOR_PIN) {}

    static BackrestMotorController *instance;
    static void backrestISR();
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
