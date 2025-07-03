#include "WiFiManager.h"

bool WiFiManager::connect()
{
    auto wifi_ssid = NVSStorage::getInstance().getCredential("wifi_ssid");
    auto wifi_password = NVSStorage::getInstance().getCredential("wifi_password");

    if (wifi_ssid.isEmpty() || wifi_password.isEmpty())
        return false;

    Serial.printf("WIFI ssid:%s pw:%s\n", wifi_ssid.c_str(), wifi_password.c_str());
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(50);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::getState()
{
    return WiFi.isConnected();
}