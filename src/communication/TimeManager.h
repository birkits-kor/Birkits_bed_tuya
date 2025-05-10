#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

class TimeManager
{
public:
    bool sync();

private:
    const char *ntp_server = "pool.ntp.org"; // Default NTP server
    const long gmt_offset_sec = 0;           // GMT offset in seconds (adjust for your time zone)
    const int daylight_offset_sec = 0;       // Daylight saving time offset in seconds
    const int maxRetries = 10;
    int retryCount = 0;
};

#endif // TIME_MANAGER_H