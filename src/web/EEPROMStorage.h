#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <EEPROM.h>

class EEPROMStorage {
public:
    void begin();
    void saveCredentials(const String &ssid, const String &password,
                         const String &device_id, const String &device_secret);
    void loadCredentials(String &ssid, String &password,
                         String &device_id, String &device_secret);
    bool isDataPresent();
    void reset();  // 전체를 0xFF로 초기화
};

#endif
