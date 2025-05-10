#ifndef NVS_STORAGE_H
#define NVS_STORAGE_H

#include <Arduino.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <String.h>

class NVSStorage
{
public:
    static NVSStorage &getInstance();

    void begin();
    void saveCredential(const String &key, const String &value);
    String getCredential(const String &key);
    void reset();

private:
    NVSStorage() = default; // 생성자 비공개
    NVSStorage(const NVSStorage &) = delete; // 복사 금지
    NVSStorage &operator=(const NVSStorage &) = delete; // 대입 금지

    nvs_handle nvsHandle;
};

#endif
