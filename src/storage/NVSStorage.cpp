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

    if (getCredential("light_control") == "")
        saveCredential("light_control", "{\"light_color\":[0,0,50.0],\"light_end_time\":\"06:00\",\"light_start_time\":\"16:00\",\"light_mode\":\"mode_a\",\"light_switch\":false}");

    if (getCredential("mode_data") == "")
        saveCredential("mode_data", "["
                                    "{\"data\":{\"bed\":{\"lower\":0,\"table\":0,\"upper\":0}},\"id\":1,\"index\":1,\"title\":\"잠자기\"},"
                                    "{\"data\":{\"bed\":{\"lower\":0,\"table\":850,\"upper\":80}},\"id\":2,\"index\":2,\"title\":\"독서\"},"
                                    "{\"data\":{\"bed\":{\"lower\":35,\"table\":0,\"upper\":0}},\"id\":3,\"index\":3,\"title\":\"다리상승\"},"
                                    "{\"data\":{\"bed\":{\"lower\":30,\"table\":0,\"upper\":30}},\"id\":4,\"index\":4,\"title\":\"ZERO G\"},"
                                    "{\"data\":{\"bed\":{\"lower\":0,\"table\":0,\"upper\":50}},\"id\":5,\"index\":5,\"title\":\"TV보기\"},"
                                    "{\"data\":{\"bed\":{\"lower\":0,\"table\":600,\"upper\":80}},\"id\":6,\"index\":6,\"title\":\"밥먹기\"}"
                                    "]");
}

void NVSStorage::saveCredential(const String &key, const String &value)
{
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK)
    {
        Serial.printf("Failed to open NVS for writing: %s\n", esp_err_to_name(err));
        return;
    }

    // 기존 값 확인
    size_t required_size = 0;
    err = nvs_get_str(nvsHandle, key.c_str(), NULL, &required_size);
    if (err == ESP_OK && required_size > 0)
    {
        char *existing_value = new char[required_size];
        nvs_get_str(nvsHandle, key.c_str(), existing_value, &required_size);

        if (value == String(existing_value))
        {
            // 값이 같으면 저장 생략
            delete[] existing_value;
            nvs_close(nvsHandle);
            return;
        }

        delete[] existing_value;
    }

    // 저장
    err = nvs_set_str(nvsHandle, key.c_str(), value.c_str());
    if (err != ESP_OK)
    {
        Serial.printf("Failed to save data to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvsHandle);
        return;
    }

    // 커밋
    err = nvs_commit(nvsHandle);
    if (err != ESP_OK)
    {
        Serial.printf("NVS commit failed: %s\n", esp_err_to_name(err));
    }
    else
    {
        Serial.printf("Save [%s]:%s\n", key.c_str(), value.c_str());
    }

    nvs_close(nvsHandle);
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
