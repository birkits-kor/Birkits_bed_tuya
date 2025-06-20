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

std::vector<ModeData> BirkitsData::getModeDataList()
{
    modeList.clear();
    String jsonStr = NVSStorage::getInstance().getCredential("mode_data");

    if (jsonStr.isEmpty())
    {
        // 비어있으면 빈 벡터 리턴
        return modeList;
    }

    StaticJsonDocument<4096> doc; // 충분한 크기 확보
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

String BirkitsData::    getAlarmDataList()
{
    String jsonStr = NVSStorage::getInstance().getCredential("alarm_data");
    if (jsonStr.isEmpty())
    {
        return ""; // 빈 벡터 반환
    }
    // Serial.println(jsonStr);
    return jsonStr;
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

void BirkitsData::saveAlarmData(String data)
{
    String alarmDataStr = NVSStorage::getInstance().getCredential("alarm_data");
    if (data == alarmDataStr)
        return;

    NVSStorage::getInstance().saveCredential("alarm_data", data);
    getAlarmDataList();
}