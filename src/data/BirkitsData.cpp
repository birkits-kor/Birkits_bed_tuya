#include "BirkitsData.h"
#include "../storage/NVSStorage.h"

BirkitsData::BirkitsData()
{
   
}

BirkitsData &BirkitsData::getInstance()
{
    static BirkitsData instance;
    return instance;
}

// void BirkitsData::setLightControlData(int h, int s, double l,
//                                       const String &endTime,
//                                       const String &startTime,
//                                       const String &mode,
//                                       bool sw)
// {
//     lightControlData.h = h;
//     lightControlData.s = s;
//     lightControlData.l = l;
//     lightControlData.light_end_time = endTime;
//     lightControlData.light_start_time = startTime;
//     lightControlData.light_mode = mode;
//     lightControlData.light_switch = sw;
// }

// void BirkitsData::getLightControlData(int &h, int &s, double &l,
//                                       String &endTime,
//                                       String &startTime,
//                                       String &mode,
//                                       bool &sw) const
// {
//     h = lightControlData.h;
//     s = lightControlData.s;
//     l = lightControlData.l;
//     endTime = lightControlData.light_end_time;
//     startTime = lightControlData.light_start_time;
//     mode = lightControlData.light_mode;
//     sw = lightControlData.light_switch;
// }

std::vector<ModeData> BirkitsData::getModeDataList() const
{
    std::vector<ModeData> modeList;

    String jsonStr = NVSStorage::getInstance().getCredential("mode_data");

    if (jsonStr.isEmpty())
    {
        // 비어있으면 빈 벡터 리턴
        return modeList;
    }

    StaticJsonDocument<2048> doc; // 충분한 크기 확보
    DeserializationError err = deserializeJson(doc, jsonStr);

    if (err)
    {
        // 파싱 실패 시 빈 벡터 리턴
        return modeList;
    }

    JsonArray array = doc.as<JsonArray>();

    for (JsonObject obj : array)
    {
        JsonObject bed = obj["data"]["bed"];

        ModeData data;
        data.lower = bed["lower"] | 0;
        data.table = bed["table"] | 0;
        data.upper = bed["upper"] | 0;
        data.id = obj["id"] | 0;
        data.index = obj["index"] | 0;
        data.title = String((const char *)obj["title"]);

        modeList.push_back(data);
    }

    return modeList;
}

std::vector<AlarmData> BirkitsData::getAlarmDataList() const
{
    std::vector<AlarmData> alarmList;

    String jsonStr = NVSStorage::getInstance().getCredential("alarm_data");

    if (jsonStr.isEmpty())
    {
        return alarmList; // 빈 벡터 반환
    }

    StaticJsonDocument<4096> doc; // 충분한 크기 확보, 알람 데이터가 좀 더 클 수 있으니 크게 잡음
    DeserializationError err = deserializeJson(doc, jsonStr);

    if (err)
    {
        return alarmList; // 파싱 실패 시 빈 벡터 반환
    }

    JsonArray array = doc.as<JsonArray>();

    for (JsonObject obj : array)
    {
        AlarmData data;

        JsonObject alarm = obj["data"]["alarm"];
        JsonObject time = alarm["time"];
        JsonObject bed = obj["data"]["bed"];

        data.active = alarm["active"] | false;
        data.time.hour = time["hour"] | 0;
        data.time.minute = time["minute"] | 0;

        // weekday 배열 파싱 (optional)
        data.weekday.clear();
        if (alarm.containsKey("weekday"))
        {
            JsonArray wdArr = alarm["weekday"].as<JsonArray>();
            for (int wd : wdArr)
            {
                data.weekday.push_back(wd);
            }
        }

        data.lower = bed["lower"] | 0;
        data.table = bed["table"] | 0;
        data.upper = bed["upper"] | 0;

        data.id = obj["id"] | 0;
        data.index = obj["index"] | 0;
        data.title = String((const char*)obj["title"]);

        alarmList.push_back(data);
    }

    return alarmList;
}

void BirkitsData::setTimerData(unsigned long timestamp, int id, bool isActive)
{
    timerData.timestamp = timestamp;
    timerData.id = id;
    timerData.isActive = isActive;
}

bool BirkitsData::getTimerData(int &id, unsigned long &timestamp) const
{
    id = timerData.id;
    timestamp = timerData.timestamp;
    return timerData.isActive;
}

void BirkitsData::setStartTime(unsigned long t)
{
    startTime = t;
}

unsigned long BirkitsData::getCurrentTime()
{
    return startTime + (millis() / 1000);
}