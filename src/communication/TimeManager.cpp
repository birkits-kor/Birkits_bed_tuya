#include "TimeManager.h"
#include <WiFi.h>
#include <sys/time.h>

TimeManager& TimeManager::getInstance()
{
    static TimeManager instance;
    return instance;
}

bool TimeManager::sync()
{
    return updateTimeFromNTP();
}

bool TimeManager::updateTimeFromNTP()
{
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    struct tm timeinfo;
    int retries = 0;

    while (!getLocalTime(&timeinfo) && retries < maxRetries)
    {
        delay(1000);
        retries++;
    }

    if (retries >= maxRetries)
    {
        Serial.println("NTP sync failed");
        return false;
    }

    _lastSyncedEpoch = time(nullptr);
    _lastSyncedMillis = millis();
    _synced = true;

    Serial.println("NTP sync successful");
    Serial.println(asctime(&timeinfo));
    return true;
}

void TimeManager::computeCurrentTime(struct tm &timeinfo)
{
    if (!_synced)
    {
        memset(&timeinfo, 0, sizeof(timeinfo));
        return;
    }

    time_t currentEpoch = _lastSyncedEpoch + (millis() - _lastSyncedMillis) / 1000;
    localtime_r(&currentEpoch, &timeinfo);
}

bool TimeManager::getDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second, int &weekday)
{
    struct tm timeinfo;
    if (WiFi.isConnected() && updateTimeFromNTP())
    {
        getLocalTime(&timeinfo);
    }
    else if (_synced)
    {
        computeCurrentTime(timeinfo);
    }
    else
    {
        return false;
    }

    year = timeinfo.tm_year + 1900;
    month = timeinfo.tm_mon + 1;
    day = timeinfo.tm_mday;
    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;
    second = timeinfo.tm_sec;
    weekday = timeinfo.tm_wday;

    return true;
}

bool TimeManager::getHourMinute(int &hour, int &minute)
{
    struct tm timeinfo;
    if (WiFi.isConnected() && updateTimeFromNTP())
    {
        getLocalTime(&timeinfo);
    }
    else if (_synced)
    {
        computeCurrentTime(timeinfo);
    }
    else
    {
        return false;
    }

    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;

    return true;
}
