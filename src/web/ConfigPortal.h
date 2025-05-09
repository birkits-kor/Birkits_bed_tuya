#ifndef CONFIG_PORTAL_H
#define CONFIG_PORTAL_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

class ConfigPortal
{
public:
    ConfigPortal();
    void begin();
    void handle(); // loop에서 필요한 처리

private:
    void startAP();
    void setupServer();

    AsyncWebServer server;
    const char *ap_ssid = "birkits";

    static const int EEPROM_SIZE = 512;
    static const int FLAG_ADDR = 0; // 설정 완료 여부 저장 위치
};

#endif
