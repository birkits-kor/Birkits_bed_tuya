#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "../storage/NVSStorage.h"

enum MotorState
{
    MOTOR_STOPPED,
    MOTOR_MOVING_FORWARD,
    MOTOR_MOVING_BACKWARD
};

class MotorController
{
public:
    virtual ~MotorController() {}

    // 상태 관련 함수들
    void begin();

protected:
    int _pin1, _pin2, _hallPin;

    // 생성자는 protected로 설정하여 직접 인스턴스를 만들지 못하도록 함
    MotorController(int pin1, int pin2, int hallPin);

private:
    void loadPosition();
    void savePosition();
};

#endif
