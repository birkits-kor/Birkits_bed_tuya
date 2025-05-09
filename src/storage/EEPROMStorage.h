#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <EEPROM.h>

class EEPROMStorage
{
public:
    static EEPROMStorage &getInstance();

    void begin();
    void saveCredentials(const String &ssid, const String &password,
                         const String &device_id, const String &device_secret);
    void loadCredentials(String &ssid, String &password,
                         String &device_id, String &device_secret);
    bool isDataPresent();
    void reset();

private:
    EEPROMStorage() = default;                                // 생성자 비공개
    EEPROMStorage(const EEPROMStorage &) = delete;            // 복사 금지
    EEPROMStorage &operator=(const EEPROMStorage &) = delete; // 대입 금지
};

#endif
