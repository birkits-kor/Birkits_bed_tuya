#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "../storage/NVSStorage.h"

class WiFiManager {
public:
    bool connect();
    bool getState();
};

#endif // WIFI_MANAGER_H