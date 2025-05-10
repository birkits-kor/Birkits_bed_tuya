#include "TimeManager.h"
#include <time.h>
#include <WiFi.h>

void TimeManager::sync() {
    configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        delay(1000);
    }
}