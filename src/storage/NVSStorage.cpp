#include "NVSStorage.h"

NVSStorage &NVSStorage::getInstance()
{
    static NVSStorage instance;
    return instance;
}

void NVSStorage::begin()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK)
    {
        Serial.println("Failed to initialize NVS");
    }
}

void NVSStorage::saveCredential(const String &key, const String &value)
{
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle); // 수정: NVS_WRITE -> NVS_READWRITE
    if (err == ESP_OK)
    {
        err = nvs_set_str(nvsHandle, key.c_str(), value.c_str());
        if (err == ESP_OK)
        {
            nvs_commit(nvsHandle);
        }
        else
        {
            Serial.println("Failed to save data to NVS");
        }
        nvs_close(nvsHandle);
    }
    else
    {
        Serial.println("Failed to open NVS for writing");
    }
}

String NVSStorage::getCredential(const String &key)
{
    String value = "";
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvsHandle); 
    if (err == ESP_OK)
    {
        size_t required_size;
        err = nvs_get_str(nvsHandle, key.c_str(), NULL, &required_size);
        if (err == ESP_OK)
        {
            char *buf = new char[required_size];
            err = nvs_get_str(nvsHandle, key.c_str(), buf, &required_size);
            if (err == ESP_OK)
            {
                value = String(buf);
            }
            delete[] buf;
        }
        else
        {
            Serial.println("Failed to read data from NVS");
        }
        nvs_close(nvsHandle);
    }
    else
    {
        Serial.println("Failed to open NVS for reading");
    }
    return value;
}

void NVSStorage::reset()
{
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle); // 수정: NVS_WRITE -> NVS_READWRITE
    if (err == ESP_OK)
    {
        nvs_erase_all(nvsHandle);
        nvs_commit(nvsHandle);
        nvs_close(nvsHandle);
    }
    else
    {
        Serial.println("Failed to open NVS for reset");
    }
}
