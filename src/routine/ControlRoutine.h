#ifndef CONTROL_ROUTINE_H
#define CONTROL_ROUTINE_H

#include <Arduino.h>
#include <map>
#include "../motor/BackrestMotorController.h"
#include "../motor/LegrestMotorController.h"
#include "../motor/TableMotorController.h"
#include "../motor/SpeakerController.h"
#include "../led/LedController.h"
#include "../led/PirSensor.h"
#include "../communication/IRDecoder.h"
#include "../communication/TimeManager.h"
#include "../data/BirkitsData.h"
#include "../data/TxMessageQueue.h"

enum class MotorWaitState
{
    NONE,                               // 아무 상태 아님
    WAIT_TABLE_ORIGIN_FOR_LEG_UP,       // 다리를 올리기 위해 테이블 원점을 기다리는 중
    WAIT_TABLE_ORIGIN_FOR_LEG_DOWN,     // 다리를 내리기 위해 테이블 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD,  // 테이블을 앞으로 밀기 위해 다리 원점을 기다리는 중
    WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD, // 테이블을 뒤로 밀기 위해 다리 원점을 기다리는 중
    WAIT_TABLE_ORIGIN_FOR_MODE,
    WAIT_LEG_ORIGIN_FOR_MODE,
    DONE_MODE
};

enum class ControlMode {
    None,
    IR,
    App
};

struct BedData {
    int bed_position = 0;
    int bed_angle = 0;
    int desk_position = 0;
};

struct SnoozeData {
    bool flag = false;
    int minutes;
    int hours;   // 스누즈 대기 시간
    int bed_lower;      // 침대 하부 위치
    int bed_table;      // 테이블 위치
    int bed_upper;      // 침대 상부 위치
    int id;
};

class ControlRoutine
{
public:
    ControlRoutine();
    void begin();
    void updatePos();
    void loopByIr(IRCommand cmd);
    void loopByApp();
    void loopLed();
    void loopAlram();
    void loopSnooze();
    static void setSnooze(int min, int l, int t, int u, int id);
    static void setBedData(const BedData& data);

private:
    static MotorWaitState state;
    IRCommand preCmd = IRCommand::NONE;
    static ControlMode controlMode;
    static BedData bedData;
    static SnoozeData snoozeData;
    bool ledState = false;
    int mode = 0;
    bool modeFlag = true;

    unsigned long ledStartMillis = 0;
    bool ledFlag = false;

    int _b, _l, _t;
};

#endif
