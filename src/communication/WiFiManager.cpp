#include "WiFiManager.h"

bool WiFiManager::connect()
{
    delay(1000);
    if (!EEPROMStorage::getInstance().isDataPresent())
        return false;

    String ssid, password, deviceId, deviceSecret;
    EEPROMStorage::getInstance().loadCredentials(ssid, password, deviceId, deviceSecret);
    Serial.printf("WIFI ssid:%s pw:%s\n", ssid.c_str(), password.c_str());
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}