#ifndef BIRKITS_DATA_H
#define BIRKITS_DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

struct ModeData
{
    int lower;      // bed.lower
    int table;      // bed.table
    int upper;      // bed.upper
    int id;
    int index;
    String title;
};

struct TimerData {
    unsigned long timestamp;  // 유닉스 타임스탬프 (초 또는 밀리초 단위)
    int id;                   // 타이머 ID
    bool isActive = false;            // 타이머 활성 상태 (true: 작동 중, false: 정지)
};

struct AlarmTime {
    int hour;
    int minute;
};

struct AlarmData {
    bool active;
    AlarmTime time;
    std::vector<int> weekday;  // 요일 배열, 예: [2,4]

    int lower;
    int table;
    int upper;

    int id;
    int index;
    String title;
};

class BirkitsData
{
public:
    static BirkitsData &getInstance();

    std::vector<ModeData> getModeDataList();
    String getAlarmDataList();
    void saveAlarmData(String data);

    void setTimerData(unsigned long timestamp, int id, bool isActive);
    bool getTimerData(int &id, unsigned long &timestamp) const;
    unsigned long getCurrentTime();
    void setStartTime(unsigned long t);

private:
    BirkitsData(); // 생성자 private
    ~BirkitsData() = default;

    unsigned long startTime;
    bool speaker_switch = false;
    TimerData timerData;
    std::vector<ModeData> modeList;
};

#endif // BIRKITS_DATA_H
