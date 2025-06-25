#include "ControlRoutine.h"

ControlMode ControlRoutine::controlMode = ControlMode::None;
MotorWaitState ControlRoutine::state = MotorWaitState::NONE;
BedData ControlRoutine::bedData;
SnoozeData ControlRoutine::snoozeData;

ControlRoutine::ControlRoutine()
{
    bedData.bed_angle = BackrestMotorController::getInstance()->getPosition();
    bedData.bed_position = LegrestMotorController::getInstance()->getPosition();
    bedData.desk_position = TableMotorController::getInstance()->getPosition();
}

void ControlRoutine::begin()
{
    BackrestMotorController::getInstance()->setupMotor();
    LegrestMotorController::getInstance()->setupMotor();
    TableMotorController::getInstance()->setupMotor();
    SpeakerController::getInstance();
    LedController::getInstance()->setup();
    LedController::getInstance()->stop();
}

void ControlRoutine::updatePos()
{
    BackrestMotorController::getInstance()->updatePos();
    LegrestMotorController::getInstance()->updatePos();
    TableMotorController::getInstance()->updatePos();
    auto b = BackrestMotorController::getInstance()->getPosition();
    auto l = LegrestMotorController::getInstance()->getPosition();
    auto t = TableMotorController::getInstance()->getPosition();

    if (_b != b || _l != l || _t != t)
    {
        _b = b;
        _l = l;
        _t = t;
        StaticJsonDocument<256> doc;
        JsonObject data = doc.createNestedObject("data");
        data["topic"] = "bed_control";
        JsonObject bedControl = data.createNestedObject("bed_control");
        bedControl["bed_angle"] = map(l, 0, LEGREST_MAX, 0, 35);
        bedControl["bed_position"] = map(b, 0, BACKREST_MAX, 0, 80);
        bedControl["desk_position"] = map(t, 0, TABLE_MAX, 0, 850);
        String result;
        serializeJson(doc, result);
        TxMessageQueue::getInstance().push(result);
    }
}

void ControlRoutine::loopByIr(IRCommand cmd)
{
    // APP에서 IR로 넘어옴
    if (controlMode != ControlMode::IR && cmd != IRCommand::NONE)
    {
        controlMode = ControlMode::IR;
        if (cmd >= IRCommand::BACKREST_UP && IRCommand::BACKREST_UP <= IRCommand::POSE6)
        {
            preCmd = IRCommand::NONE;
            BackrestMotorController::getInstance()->stopMotor();
            LegrestMotorController::getInstance()->stopMotor();
            TableMotorController::getInstance()->stopMotor();
        }
    }

    if (controlMode != ControlMode::IR) // APP 제어 중
        return;

    if (state != MotorWaitState::NONE)
    {
        switch (state)
        {
        case MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_UP:
            if (TableMotorController::getInstance()->getPosition() < 30)
            {
                LegrestMotorController::getInstance()->moveTo(LEGREST_MAX);
                state = MotorWaitState::NONE;
            }
            break;
        case MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_DOWN:
            if (TableMotorController::getInstance()->getPosition() < 30)
            {
                LegrestMotorController::getInstance()->moveTo(0);
                state = MotorWaitState::NONE;
            }
            break;
        case MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD:
            if (LegrestMotorController::getInstance()->getPosition() < 30)
            {
                TableMotorController::getInstance()->moveTo(TABLE_MAX);
                state = MotorWaitState::NONE;
            }
            break;
        case MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD:
            if (LegrestMotorController::getInstance()->getPosition() < 30)
            {
                TableMotorController::getInstance()->moveTo(0);
                state = MotorWaitState::NONE;
            }
            break;
        }
    }

    switch (cmd)
    {
    case IRCommand::NONE:
        break;
    case IRCommand::STOP:
        preCmd = cmd;
        BackrestMotorController::getInstance()->stopMotor();
        LegrestMotorController::getInstance()->stopMotor();
        TableMotorController::getInstance()->stopMotor();
        break;

    case IRCommand::BACKREST_UP:
        if (preCmd != IRCommand::BACKREST_UP)
        {
            BackrestMotorController::getInstance()->moveTo(BACKREST_MAX);
            preCmd = cmd;
        }
        else
        {
            BackrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::BACKREST_DOWN:
        if (preCmd != IRCommand::BACKREST_DOWN)
        {
            BackrestMotorController::getInstance()->moveTo(0);
            preCmd = cmd;
        }
        else
        {
            BackrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::LEG_UP:
        if (preCmd != IRCommand::LEG_UP)
        {
            auto p = TableMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_UP;
                TableMotorController::getInstance()->moveTo(0);
            }
            else
            {
                LegrestMotorController::getInstance()->moveTo(LEGREST_MAX);
            }
            preCmd = cmd;
        }
        else
        {
            LegrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::LEG_DOWN:
        if (preCmd != IRCommand::LEG_DOWN)
        {
            auto p = TableMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_DOWN;
                TableMotorController::getInstance()->moveTo(0);
            }
            else
            {
                LegrestMotorController::getInstance()->moveTo(0);
            }
            preCmd = cmd;
        }
        else
        {
            LegrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::TABLE_FORWARD:
        if (preCmd != IRCommand::TABLE_FORWARD)
        {
            auto p = LegrestMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD;
                LegrestMotorController::getInstance()->moveTo(0);
            }
            else
            {
                TableMotorController::getInstance()->moveTo(TABLE_MAX);
            }
            preCmd = cmd;
        }
        else
        {
            TableMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::TABLE_BACKWARD:
        if (preCmd != IRCommand::TABLE_BACKWARD)
        {
            auto p = LegrestMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD;
                LegrestMotorController::getInstance()->moveTo(0);
            }
            else
            {
                TableMotorController::getInstance()->moveTo(0);
            }
            preCmd = cmd;
        }
        else
        {
            TableMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }

    case IRCommand::POSE1:
        mode = 1;
        break;

    case IRCommand::POSE2:
        mode = 2;
        break;

    case IRCommand::POSE3:
        mode = 3;
        break;

    case IRCommand::POSE4:
        mode = 4;
        break;

    case IRCommand::POSE5:
        mode = 5;
        break;

    case IRCommand::POSE6:
        mode = 6;
        break;

    case IRCommand::SPEAKER:
        if (SpeakerController::getInstance()->isOn())
            SpeakerController::getInstance()->off();
        else
            SpeakerController::getInstance()->on();
        break;

    case IRCommand::LIGHT:
        if (!ledState)
        {
            LedController::getInstance()->run();
            ledState = true;
        }
        else
        {
            LedController::getInstance()->stop();
            ledState = false;
        }
        break;
    default:
        break;
    }

    // 원점 설정 여부에 따라 로직 구현 다름름
    if (mode && modeFlag)
    {
    }
}

void ControlRoutine::loopByApp()
{
    if (controlMode != ControlMode::App || state == MotorWaitState::DONE_MODE) // APP 제어 중
        return;

    if (state == MotorWaitState::WAIT_TABLE_ORIGIN_FOR_MODE && TableMotorController::getInstance()->getPosition() < 30)
    {
        BackrestMotorController::getInstance()->moveTo(bedData.bed_position);
        LegrestMotorController::getInstance()->moveTo(bedData.bed_angle);
        TableMotorController::getInstance()->moveTo(bedData.desk_position);
        state = MotorWaitState::DONE_MODE;
        return;
    }

    if (state == MotorWaitState::WAIT_LEG_ORIGIN_FOR_MODE && LegrestMotorController::getInstance()->getPosition() < 30)
    {
        BackrestMotorController::getInstance()->moveTo(bedData.bed_position);
        LegrestMotorController::getInstance()->moveTo(bedData.bed_angle);
        TableMotorController::getInstance()->moveTo(bedData.desk_position);
        state = MotorWaitState::DONE_MODE;
        return;
    }

    // 충돌 예방
    if (bedData.bed_angle > 0)
        bedData.desk_position = 0;
    else if (bedData.desk_position > 0)
        bedData.bed_angle = 0;

    if (bedData.bed_angle != 0 && bedData.desk_position == 0 && TableMotorController::getInstance()->getPosition() > 30) // 테이블 먼저 움직이고 다른거 하기기
    {
        TableMotorController::getInstance()->moveTo(0);
        state = MotorWaitState::WAIT_TABLE_ORIGIN_FOR_MODE;
        return;
    }

    if (bedData.desk_position != 0 && bedData.bed_angle == 0 && LegrestMotorController::getInstance()->getPosition() > 30) // 테이블 먼저 움직이고 다른거 하기기
    {
        LegrestMotorController::getInstance()->moveTo(0);
        state = MotorWaitState::WAIT_LEG_ORIGIN_FOR_MODE;
        return;
    }
    Serial.println("move");
    BackrestMotorController::getInstance()->moveTo(bedData.bed_position);
    LegrestMotorController::getInstance()->moveTo(bedData.bed_angle);
    TableMotorController::getInstance()->moveTo(bedData.desk_position);
    state = MotorWaitState::DONE_MODE;
}

void ControlRoutine::loopLed()
{
    if (PirSensor::getInstance().get() && LedController::getInstance()->getSw() && ledFlag == false)
    {
        int h, m;
        TimeManager::getInstance().getHourMinute(h, m);
        if (LedController::getInstance()->isWithinLightTime(h, m))
        {
            ledFlag = true;
            LedController::getInstance()->reset();
            ledStartMillis = millis();
        }
    }

    if (ledFlag)
    {
        if (millis() - ledStartMillis < 5000)
        {
            LedController::getInstance()->run();
        }
        else
        {
            ledFlag = false; // 5초 경과 시 종료
            LedController::getInstance()->stop();
        }
    }
}

void ControlRoutine::loopAlram()
{
    std::vector<AlarmData> alarms;
    auto data = BirkitsData::getInstance().getAlarmDataList();
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, data);
    if (error)
        return;

    JsonArray arr = doc.as<JsonArray>();

    for (JsonObject obj : arr)
    {
        AlarmData alarmData;

        JsonObject data = obj["data"];
        JsonObject alarm = data["alarm"];
        JsonObject time = alarm["time"];
        JsonObject bed = data["bed"];

        // 기본 데이터
        alarmData.id = obj["id"].as<int>();
        alarmData.index = obj["index"].as<int>();
        alarmData.title = String(obj["title"].as<const char *>());

        // Alarm
        alarmData.active = alarm["active"].as<bool>();
        alarmData.time.hour = time["hour"].as<int>();
        alarmData.time.minute = time["minute"].as<int>();

        // weekday (std::vector로 옮기기)
        for (int weekday : alarm["weekday"].as<JsonArray>())
        {
            alarmData.weekday.push_back(weekday);
        }

        // Bed
        alarmData.lower = bed["lower"].as<int>();
        alarmData.table = bed["table"].as<int>();
        alarmData.upper = bed["upper"].as<int>();

        alarms.push_back(alarmData);
    }

    int nowYear, nowMonth, nowDay, nowHour, nowMinute, nowSecond, nowWeekday;
    TimeManager::getInstance().getDateTime(nowYear, nowMonth, nowDay, nowHour, nowMinute, nowSecond, nowWeekday);
    Serial.printf("현재 시각: %04d-%02d-%02d %02d:%02d:%02d (weekday=%d)\n",
                  nowYear, nowMonth, nowDay, nowHour, nowMinute, nowSecond, nowWeekday);
    // 결과 출력
    for (const AlarmData &a : alarms)
    {
        bool weekdayMatch = false;
        for (int w : a.weekday)
        {
            if (w == nowWeekday)
            {
                weekdayMatch = true;
                break;
            }
        }
        bool timeMatch = (a.time.hour == nowHour && a.time.minute == nowMinute);
        bool isTriggered = a.active && weekdayMatch && timeMatch;

        if (isTriggered)
        {
            Serial.printf("Alram on [ID:%d Title:%s]\n", a.id, a.title);
            BedData bed;
            bed.bed_angle = a.upper;
            bed.bed_position = a.lower;
            bed.desk_position = a.table;
            setBedData(bed);
            StaticJsonDocument<256> doc;

            JsonObject data = doc.createNestedObject("data");
            data["topic"] = "alarm_control";

            JsonObject alarm_control = data.createNestedObject("alarm_control");
            alarm_control["id"] = a.id;
            alarm_control["state"] = "done";
            String jsonStr;
            serializeJson(doc, jsonStr);
            TxMessageQueue::getInstance().push(jsonStr);
        }
    }
}

void ControlRoutine::loopSnooze()
{
    int h, m;
    TimeManager::getInstance().getHourMinute(h, m);
    if (snoozeData.flag && snoozeData.hours == h && snoozeData.minutes == m)
    {
        snoozeData.flag = false;
        Serial.printf("Snooze on [ID:%d]\n", snoozeData.id);
        BedData bed;
        bed.bed_angle = snoozeData.bed_upper;
        bed.bed_position = snoozeData.bed_lower;
        bed.desk_position = snoozeData.bed_table;
        setBedData(bed);
        StaticJsonDocument<256> doc;

        JsonObject data = doc.createNestedObject("data");
        data["topic"] = "alarm_control";

        JsonObject alarm_control = data.createNestedObject("alarm_control");
        alarm_control["id"] = snoozeData.id;
        alarm_control["state"] = "done";
        String jsonStr;
        serializeJson(doc, jsonStr);
        TxMessageQueue::getInstance().push(jsonStr);
    }
}

void ControlRoutine::setBedData(const BedData &data)
{
    // 데이터 변경 시 일시 정지 후 데이터 처리리
    bedData = data;
    controlMode = ControlMode::App;
    BackrestMotorController::getInstance()->stopMotor();
    LegrestMotorController::getInstance()->stopMotor();
    TableMotorController::getInstance()->stopMotor();
    state = MotorWaitState::NONE;
    Serial.printf("%d %d %d\n", bedData.bed_angle, bedData.bed_position, bedData.desk_position);
}
void ControlRoutine::setSnooze(int min, int l, int t, int u, int id)
{
    snoozeData.flag = true;
    snoozeData.bed_lower = l;
    snoozeData.bed_table = t;
    snoozeData.bed_upper = u;
    snoozeData.id = id;

    int h, m;
    TimeManager::getInstance().getHourMinute(h, m);
    m += min;
    if (m >= 60)
    {
        h++;
        min -= 60;
    }
    if (h == 24)
        h = 0;

    snoozeData.hours = h;
    snoozeData.minutes = m;
}