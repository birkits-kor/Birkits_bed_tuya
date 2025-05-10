#include "TimeManager.h"
#include <time.h>
#include <WiFi.h>

bool TimeManager::sync()
{
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    Serial.print("Waiting for NTP time sync: ");
    while (time(nullptr) < 8 * 3600 * 2)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Time synchronized");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        Serial.print("Current time: ");
        Serial.println(asctime(&timeinfo));
        return true;
    }
    else
    {
        Serial.println("Failed to obtain local time");
        return false;
    }
}