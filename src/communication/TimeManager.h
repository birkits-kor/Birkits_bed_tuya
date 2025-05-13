#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <time.h>

class TimeManager
{
public:
    static TimeManager& getInstance();

    bool sync();
    bool getDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second, int &weekday);
    bool getHourMinute(int &hour, int &minute);

private:
    TimeManager() = default;

    bool updateTimeFromNTP();
    void computeCurrentTime(struct tm &timeinfo);

    const char* ntp_server = "pool.ntp.org";
    const long gmt_offset_sec = 9 * 3600; // 한국 기준 GMT+9
    const int daylight_offset_sec = 0;
    const int maxRetries = 10;

    bool _synced = false;
    time_t _lastSyncedEpoch = 0;
    unsigned long _lastSyncedMillis = 0;
};

#endif // TIME_MANAGER_H
