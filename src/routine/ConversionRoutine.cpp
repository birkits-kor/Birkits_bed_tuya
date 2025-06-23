#include "ConversionRoutine.h"
#include "../data/TxMessageQueue.h"
#include "../data/BirkitsData.h"

#include "../motor/BackrestMotorController.h"
#include "../motor/LegrestMotorController.h"
#include "../motor/TableMotorController.h"
#include "../motor/SpeakerController.h"
#include "../led/LedController.h"

#include "ControlRoutine.h"

void ConversionRoutine::begin()
{
    jsonConversionHandler.registerCallback("init", [this](const String &payload)
                                           { this->intfunc(payload); });
    jsonConversionHandler.registerCallback("speaker_switch", [this](const String &payload)
                                           { this->speakerfunc(payload); });
    jsonConversionHandler.registerCallback("light_control", [this](const String &payload)
                                           { this->lightfunc(payload); });
    jsonConversionHandler.registerCallback("bed_control", [this](const String &payload)
                                           { this->bedfunc(payload); });
    jsonConversionHandler.registerCallback("stop", [this](const String &payload)
                                           { this->stopfunc(payload); });
    jsonConversionHandler.registerCallback("alarm_data", [this](const String &payload)
                                           { this->alarmfunc(payload); });
    jsonConversionHandler.registerCallback("alarm_control", [this](const String &payload)
                                           { this->alarmControlfunc(payload); });
    jsonConversionHandler.registerCallback("mode_data", [this](const String &payload)
                                           { this->modefunc(payload); });
}

void ConversionRoutine::loop()
{
    jsonConversionHandler.loop();
}

void ConversionRoutine::intfunc(const String &payload)
{
    Serial.println("[init]");
    auto bedControlData = makeBedControlData("init");
    TxMessageQueue::getInstance().push(bedControlData);

    auto lightControlData = makeLightControlData("init");
    TxMessageQueue::getInstance().push(lightControlData);

    auto speakerSwData = makeSpeakerSwData("init");
    TxMessageQueue::getInstance().push(speakerSwData);

    auto modeData = makeModeData("init");
    TxMessageQueue::getInstance().push(modeData);
    auto alramData = makeAlarmData("init");
    TxMessageQueue::getInstance().push(alramData);
    int id;
    unsigned long timestamp;
    if (BirkitsData::getInstance().getTimerData(id, timestamp))
    {
        auto timerData = makeTimerData("init");
        TxMessageQueue::getInstance().push(timerData);
    }
}

void ConversionRoutine::speakerfunc(const String &payload)
{
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (doc.containsKey("speaker_switch")) {
        bool v = doc["speaker_switch"];
        if(v)
            SpeakerController::getInstance()->on();
        else
            SpeakerController::getInstance()->off();
    }
}

void ConversionRoutine::lightfunc(const String &payload)
{
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("lightfunc() failed: "));
        Serial.println(error.f_str());
        return;
    }
    JsonObject lightControl = doc["light_control"];
    String output;
    serializeJson(lightControl, output);
    LedController::getInstance()->saveData(output);
}

void ConversionRoutine::bedfunc(const String &payload)
{
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print(F("JSON parsing failed: "));
        Serial.println(error.f_str());
        return;
    }
    JsonObject bed = doc["bed_control"];
    BedData data;
    data.bed_angle     = map(bed["bed_angle"], 0, 35 ,0 ,LEGREST_MAX);
    data.bed_position  = map(bed["bed_position"], 0, 80 ,0 ,BACKREST_MAX);
    data.desk_position = map(bed["desk_position"], 0, 850 ,0 ,TABLE_MAX);
    ControlRoutine::setBedData(data);
}

void ConversionRoutine::stopfunc(const String &payload)
{
    BackrestMotorController::getInstance()->stopMotor();
    LegrestMotorController::getInstance()->stopMotor();
    TableMotorController::getInstance()->stopMotor();
}

void ConversionRoutine::alarmfunc(const String &payload)
{
    Serial.println(payload);
    StaticJsonDocument<4096> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
        Serial.print(F("lightfunc() failed: "));
        Serial.println(error.f_str());
        return;
    }
    JsonArray alarmArray = doc["alarm_data"];
    String output;
    serializeJson(alarmArray, output);
    BirkitsData::getInstance().saveAlarmData(output);
}

void ConversionRoutine::alarmControlfunc(const String &payload)
{
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
        Serial.print("JSON 파싱 실패: ");
        Serial.println(err.c_str());
        return;
    }

    JsonObject alarmCtrl = doc["alarm_control"];
    int waitMinutes = alarmCtrl["wait_minutes"] | 0;
    int id = alarmCtrl["id"] | 0;

    JsonObject bed = alarmCtrl["bed"];
    int lower = bed["lower"] | 0;
    int table = bed["table"] | 0;
    int upper = bed["upper"] | 0;


    Serial.printf("[%d] wait_minutes: %d\n",id, waitMinutes);
    Serial.printf("bed → lower: %d, table: %d, upper: %d\n", lower, table, upper);
    ControlRoutine::setSnooze(waitMinutes, lower, table, upper, id);
}

void ConversionRoutine::modefunc(const String &payload)
{
    StaticJsonDocument<4096> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
        Serial.println("JSON 파싱 오류");
        return ;
    }

    JsonArray modeArray = doc["data"]["mode_data"];
    if (!modeArray)
    {
        Serial.println("mode_data 없음");
        return ;
    }

    String output;
    serializeJson(modeArray, output);
    BirkitsData::getInstance().saveModeData(output);
}

String ConversionRoutine::makeBedControlData(String topic)
{
    auto b = BackrestMotorController::getInstance()->getPosition();
    auto l = LegrestMotorController::getInstance()->getPosition();
    auto t = TableMotorController::getInstance()->getPosition();
    StaticJsonDocument<256> doc;
    JsonObject data = doc.createNestedObject("data");
    data["topic"] = topic;
    JsonObject bedControl = data.createNestedObject("bed_control");
    bedControl["bed_angle"] = map(l, 0, LEGREST_MAX, 0, 35);
    bedControl["bed_position"] = map(b, 0, BACKREST_MAX ,0 ,80);
    bedControl["desk_position"] = map(t, 0, TABLE_MAX, 0, 850);
    String result;
    serializeJson(doc, result);
    Serial.println(result); // 디버깅용
    return result;
}

String ConversionRoutine::makeLightControlData(String topic)
{
    uint16_t h, s, v;
    String endTime, startTime, mode;
    bool sw;

    // 현재 저장된 lightControl 데이터 가져오기
    LedController::getInstance()->getLightControlData(h, s, v, endTime, startTime, mode, sw);

    // JSON 구성
    StaticJsonDocument<512> doc;

    JsonObject data = doc.createNestedObject("data");
    data["topic"] = topic;

    JsonObject light = data.createNestedObject("light_control");
    JsonArray color = light.createNestedArray("light_color");
    color.add(h);
    color.add(s);
    color.add(v);

    light["light_end_time"] = endTime;
    light["light_start_time"] = startTime;
    light["light_mode"] = mode;
    light["light_switch"] = sw;

    String result;
    serializeJson(doc, result);
    Serial.println(result); // 디버깅용
    return result;
}

String ConversionRoutine::makeSpeakerSwData(String topic)
{
    // BirkitsData에서 speaker_switch 값 가져오기
    bool sw = SpeakerController::getInstance()->isOn();

    // JSON 생성
    StaticJsonDocument<256> doc;
    JsonObject data = doc.createNestedObject("data");
    data["topic"] = topic;
    data["speaker_switch"] = sw;

    // JSON 직렬화
    String result;
    serializeJson(doc, result);
    Serial.println(result); // 디버깅용
    return result;
}

String ConversionRoutine::makeModeData(String topic)
{
    auto dataList = BirkitsData::getInstance().getModeDataList();

    StaticJsonDocument<4096> doc;
    JsonObject root = doc.createNestedObject("data");

    root["topic"] = topic;
    JsonArray modeArray = root.createNestedArray("mode_data");

    for (const ModeData &m : dataList)
    {
        JsonObject modeObj = modeArray.createNestedObject();
        JsonObject bedObj = modeObj.createNestedObject("data").createNestedObject("bed");

        bedObj["lower"] = m.lower;
        bedObj["table"] = m.table;
        bedObj["upper"] = m.upper;

        modeObj["id"] = m.id;
        modeObj["index"] = m.index;
        modeObj["title"] = m.title;
    }

    String result;
    serializeJson(doc, result);
    Serial.println(result); // 디버깅용
    return result;
}

String ConversionRoutine::makeAlarmData(String topic)
{
    String alarmListStr = BirkitsData::getInstance().getAlarmDataList();
    StaticJsonDocument<4096> doc;
    JsonObject outer = doc.to<JsonObject>();
    JsonArray alarmArray = outer.createNestedArray("alarm_data");
    outer["topic"] = topic;
    StaticJsonDocument<4096> alarmDoc;
    DeserializationError error = deserializeJson(alarmDoc, alarmListStr);
    if (error)
    {
        Serial.print(F("Failed to parse alarm list: "));
        Serial.println(error.f_str());
        return "{}";
    }

    JsonArray parsedAlarmArray = alarmDoc.as<JsonArray>();
    for (JsonObject alarmItem : parsedAlarmArray)
    {
        alarmArray.add(alarmItem);
    }

    String result;
    serializeJson(outer, result);
    Serial.println(result); // 디버깅용
    return result;
}

String ConversionRoutine::makeTimerData(String topic)
{
    int timerId;
    unsigned long endTimestamp; // 타이머가 만료되는 시간 (목표 시간)
    bool isActive = BirkitsData::getInstance().getTimerData(timerId, endTimestamp);

    unsigned long currentTime = BirkitsData::getInstance().getCurrentTime();

    // 남은 시간 계산 (초 단위)
    unsigned long remaining = (endTimestamp > currentTime) ? (endTimestamp - currentTime) : 0;

    // 남은 시간을 HH:MM 포맷으로 변환
    int hour = remaining / 3600;
    int minute = (remaining % 3600) / 60;

    char timeStr[6];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", hour, minute);

    StaticJsonDocument<256> doc;
    JsonObject data = doc.createNestedObject("data");
    data["topic"] = topic;

    JsonObject timerControl = data.createNestedObject("timer_control");
    timerControl["timer_data"] = String(timeStr);
    timerControl["timer_id"] = timerId;

    String result;
    serializeJson(doc, result);
    Serial.println(result);
    return result;
}