#include "EEPROMStorage.h"

#define EEPROM_SIZE 256
#define SSID_ADDR 0
#define SSID_LEN 32
#define PASSWORD_ADDR (SSID_ADDR + SSID_LEN)
#define PASSWORD_LEN 64
#define DEVICE_ID_ADDR (PASSWORD_ADDR + PASSWORD_LEN)
#define DEVICE_ID_LEN 22
#define DEVICE_SECRET_ADDR (DEVICE_ID_ADDR + DEVICE_ID_LEN)
#define DEVICE_SECRET_LEN 16

EEPROMStorage &EEPROMStorage::getInstance()
{
    static EEPROMStorage instance;
    return instance;
}

void EEPROMStorage::begin()
{
    EEPROM.begin(EEPROM_SIZE);
}

void EEPROMStorage::saveCredentials(const String &ssid, const String &password,
                                    const String &device_id, const String &device_secret)
{
    Serial.println("Saving credentials:");
    Serial.printf("SSID: %s\n", ssid.c_str());
    Serial.printf("Password: %s\n", password.c_str());
    Serial.printf("Device ID: %s\n", device_id.c_str());
    Serial.printf("Device Secret: %s\n", device_secret.c_str());

    for (int i = 0; i < SSID_LEN; ++i)
        EEPROM.write(SSID_ADDR + i, i < ssid.length() ? ssid[i] : 0x00);

    for (int i = 0; i < PASSWORD_LEN; ++i)
        EEPROM.write(PASSWORD_ADDR + i, i < password.length() ? password[i] : 0x00);

    for (int i = 0; i < DEVICE_ID_LEN; ++i)
        EEPROM.write(DEVICE_ID_ADDR + i, i < device_id.length() ? device_id[i] : 0x00);

    for (int i = 0; i < DEVICE_SECRET_LEN; ++i)
        EEPROM.write(DEVICE_SECRET_ADDR + i, i < device_secret.length() ? device_secret[i] : 0x00);

    EEPROM.commit();
}

void EEPROMStorage::loadCredentials(String &ssid, String &password,
                                    String &device_id, String &device_secret)
{
    char buffer[65];

    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < SSID_LEN; ++i)
        buffer[i] = EEPROM.read(SSID_ADDR + i);
    ssid = String(buffer);

    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < PASSWORD_LEN; ++i)
        buffer[i] = EEPROM.read(PASSWORD_ADDR + i);
    password = String(buffer);

    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < DEVICE_ID_LEN; ++i)
        buffer[i] = EEPROM.read(DEVICE_ID_ADDR + i);
    device_id = String(buffer);

    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < DEVICE_SECRET_LEN; ++i)
        buffer[i] = EEPROM.read(DEVICE_SECRET_ADDR + i);
    device_secret = String(buffer);
}

bool EEPROMStorage::isDataPresent()
{
    return EEPROM.read(SSID_ADDR) != 0xFF &&
           EEPROM.read(PASSWORD_ADDR) != 0xFF &&
           EEPROM.read(DEVICE_ID_ADDR) != 0xFF &&
           EEPROM.read(DEVICE_SECRET_ADDR) != 0xFF;
}

void EEPROMStorage::reset()
{
    for (int i = 0; i < EEPROM_SIZE; ++i)
        EEPROM.write(i, 0xFF);
    EEPROM.commit();
}
