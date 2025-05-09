#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "../storage/EEPROMStorage.h"

class WiFiManager {
public:
    bool connect();
};

#endif // WIFI_MANAGER_H