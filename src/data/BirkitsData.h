#ifndef BIRKITS_DATA_H
#define BIRKITS_DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

struct LightControlData
{
    int h, s;
    double l;                // RGB 또는 HSL 등 색상 3값
    String light_end_time;   // "06:00"
    String light_start_time; // "16:00"
    String light_mode;       // "mode_a"
    bool light_switch;       // true / false
};

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
    void getMotorPosition(int &b, int &l, int &t);

    void getLightControlData(int &h, int &s, double &l,
                             String &endTime,
                             String &startTime,
                             String &mode,
                             bool &sw) const;
    void setLightControlData(int h, int s, double l,
                             const String &endTime,
                             const String &startTime,
                             const String &mode,
                             bool sw);

    void setSpeakerSwitch(bool sw);
    bool getSpeakerSwitch() const;
    std::vector<ModeData> getModeDataList() const;
    std::vector<AlarmData> getAlarmDataList() const;
    void setTimerData(unsigned long timestamp, int id, bool isActive);
    bool getTimerData(int &id, unsigned long &timestamp) const;
    unsigned long getCurrentTime();

    void setStartTime(unsigned long t);

private:
    BirkitsData(); // 생성자 private
    ~BirkitsData() = default;

    LightControlData lightControlData;
    unsigned long startTime;
    bool speaker_switch = false;
    TimerData timerData;
};

#endif // BIRKITS_DATA_H
