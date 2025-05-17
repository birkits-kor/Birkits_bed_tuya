#ifndef MOTOR_ROUTINE_H
#define MOTOR_ROUTINE_H

#include <Arduino.h>
#include <map>
#include "../motor/BackrestMotorController.h"
#include "../motor/LegrestMotorController.h"
#include "../motor/TableMotorController.h"
#include "../motor/SpeakerController.h"
#include "../communication/IRDecoder.h"

enum class MotorWaitState {
    NONE,                               // 아무 상태 아님
    WAIT_TABLE_ORIGIN_FOR_LEG_UP,      // 다리를 올리기 위해 테이블 원점을 기다리는 중
    WAIT_TABLE_ORIGIN_FOR_LEG_DOWN,    // 다리를 내리기 위해 테이블 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD, // 테이블을 앞으로 밀기 위해 다리 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD // 테이블을 뒤로 밀기 위해 다리 원점을 기다리는 중
};

class MotorRoutine
{
public:
    MotorRoutine();
    void begin();
    void updatePos();
    void loopByIr();

    void addMotor(const String &motorName, MotorController *motor);
    void moveForward(const String &motorName);
    void moveBackward(const String &motorName);
    void stopMotor(const String &motorName);
    int getSensorValue(const String &motorName);
    void stopAllMotors();

private:
    IRDecoder irDecoder;
    IRCommand preCmd = IRCommand::NONE;
    MotorWaitState state = MotorWaitState::NONE;
};

#endif
