#ifndef MOTOR_ROUTINE_H
#define MOTOR_ROUTINE_H

#include <Arduino.h>
#include <map>
#include "../motor/BackrestMotorController.h"
#include "../motor/LegrestMotorController.h"
#include "../motor/TableMotorController.h"
#include "../motor/SpeakerController.h"
#include "../led/LedController.h"
#include "../communication/IRDecoder.h"

enum class MotorWaitState
{
    NONE,                              // 아무 상태 아님
    WAIT_TABLE_ORIGIN_FOR_LEG_UP,      // 다리를 올리기 위해 테이블 원점을 기다리는 중
    WAIT_TABLE_ORIGIN_FOR_LEG_DOWN,    // 다리를 내리기 위해 테이블 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD, // 테이블을 앞으로 밀기 위해 다리 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD, // 테이블을 뒤로 밀기 위해 다리 원점을 기다리는 중
    WAIT_TABLE_ORIGIN_FOR_MODE,  
    WAIT_LEG_ORIGIN_FOR_MODE    
};

class MotorRoutine
{
public:
    MotorRoutine();
    void begin();
    void updatePos();
    void loopByIr(IRCommand cmd);

private:
    MotorWaitState state = MotorWaitState::NONE;
    IRCommand preCmd = IRCommand::NONE;
    bool ledState = false;
    int mode = 0;
    bool modeFlag = true;
    void doMode(int n);
};

#endif
