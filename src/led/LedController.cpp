#include "LedController.h"
#include "../storage/NVSStorage.h"

LedController *LedController::instance = nullptr;

LedController::LedController()
    : _strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800),
      _strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800)
{
}

LedController *LedController::getInstance()
{
    if (instance == nullptr)
        instance = new LedController();
    return instance;
}

void LedController::setup()
{
    _strip1.begin();
    _strip1.show(); // 모든 LED 꺼짐
    _strip2.begin();
    _strip2.show(); // 모든 LED 꺼짐
    loadSavedData();
    hsvToRgb(lightControlData.h, lightControlData.s, lightControlData.l, _r, _g, _b);
}

void LedController::set(uint16_t h, uint16_t s, uint8_t v, uint8_t mode,
                        uint8_t startHour, uint8_t startMinute,
                        uint8_t endHour, uint8_t endMinute)
{
    // _h = h;
    // _s = s;
    // _v = v;
    // _mode = mode;
    // _startHour = startHour;
    // _startMinute = startMinute;
    // _endHour = endHour;
    // _endMinute = endMinute;

    // hsvToRgb(_h, _s, _v, _r, _g, _b);
    // saveData();
    // reset();
}
void LedController::reset()
{
    _count = 0;
}

void LedController::stop()
{
    for (int i = 0; i < LED_COUNT; ++i)
    {
        _strip1.setPixelColor(i, 0, 0, 0);
        _strip2.setPixelColor(i, 0, 0, 0);
    }
    _strip1.show();
    _strip2.show();
}


void LedController::getLightControlData(int &h, int &s, double &l,
                                      String &endTime,
                                      String &startTime,
                                      String &mode,
                                      bool &sw) const
{
    h = lightControlData.h;
    s = lightControlData.s;
    l = lightControlData.l;
    endTime = lightControlData.light_end_time;
    startTime = lightControlData.light_start_time;
    mode = lightControlData.light_mode;
    sw = lightControlData.light_switch;
}

void LedController::run()
{
    ++_count;

    if (lightControlData.light_mode == "mode_a")
        mode1();
    else if (lightControlData.light_mode == "mode_b")
        mode2();
    else if (lightControlData.light_mode == "mode_c")
        mode3();
    else if (lightControlData.light_mode == "mode_d")
        mode4();
}

void LedController::mode1()
{
    // 모드 1: 단순 ON (지정한 색으로 켜짐)
    for (int i = 0; i < LED_COUNT; ++i)
    {
        _strip1.setPixelColor(i, _r, _g, _b);
        _strip2.setPixelColor(i, _r, _g, _b);
    }
    _strip1.show();
    _strip2.show();
}

void LedController::mode2()
{
    // 모드 2: 디밍 (2초 주기 밝기 변화)
    uint8_t phase = (_count % 20); // 2초 주기 (100ms * 20)
    uint8_t brightness = (phase < 10) ? (phase * 10) : ((20 - phase) * 10);
    uint8_t r, g, b;
    hsvToRgb(lightControlData.h, lightControlData.s, brightness, r, g, b);
    for (int i = 0; i < LED_COUNT; ++i)
    {
        _strip1.setPixelColor(i, r, g, b);
        _strip2.setPixelColor(i, r, g, b);
    }
    _strip1.show();
    _strip2.show();
}

void LedController::mode3()
{
    // 모드 3: 레인보우
    for (int i = 0; i < LED_COUNT; ++i)
    {
        uint32_t color = _strip1.ColorHSV((_count * 10 + i * 10) % 65536);
        _strip1.setPixelColor(i, color);
        color = _strip2.ColorHSV((_count * 10 + i * 10) % 65536);
        _strip2.setPixelColor(i, color);
    }
    _strip1.show();
    _strip2.show();
}

void LedController::mode4()
{
    // 모드 4: 웨이브
    for (int i = 0; i < LED_COUNT; ++i)
    {
        uint32_t color = _strip1.ColorHSV((_count * 20 + i * 500) % 65536);
        _strip1.setPixelColor(i, color);
        color = _strip2.ColorHSV((_count * 20 + i * 500) % 65536);
        _strip2.setPixelColor(i, color);
    }
    _strip1.show();
    _strip2.show();
}

void LedController::loadSavedData()
{
    String lightDataStr = NVSStorage::getInstance().getCredential("light_control");
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, lightDataStr);

    if (!error)
    {
        JsonObject obj = doc.as<JsonObject>();

        JsonArray color = obj["light_color"];
        lightControlData.h = color[0];
        lightControlData.s = color[1];
        lightControlData.l = color[2];

        lightControlData.light_end_time = obj["light_end_time"] | "";
        lightControlData.light_start_time = obj["light_start_time"] | "";
        lightControlData.light_mode = obj["light_mode"] | "";
        lightControlData.light_switch = obj["light_switch"] | false;
    }
    else
    {
        // 파싱 실패 시 기본값 설정 (옵션)
        lightControlData = {
            0,
            0,
            50.0,
            "14:00",
            "06:00",
            "none",
            false};
    }
}

void LedController::saveData()
{
}

void LedController::hsvToRgb(uint16_t h, uint16_t s, double l, uint8_t &r, uint8_t &g, uint8_t &b)
{
    float hf = (float)h / 360.0f;
    float sf = (float)s / 1000.0f;
    float vf = (float)l / 100.0f;

    int i = int(hf * 6);
    float f = hf * 6 - i;
    float p = vf * (1 - sf);
    float q = vf * (1 - f * sf);
    float t = vf * (1 - (1 - f) * sf);

    float rf, gf, bf;
    switch (i % 6)
    {
    case 0:
        rf = vf;
        gf = t;
        bf = p;
        break;
    case 1:
        rf = q;
        gf = vf;
        bf = p;
        break;
    case 2:
        rf = p;
        gf = vf;
        bf = t;
        break;
    case 3:
        rf = p;
        gf = q;
        bf = vf;
        break;
    case 4:
        rf = t;
        gf = p;
        bf = vf;
        break;
    case 5:
        rf = vf;
        gf = p;
        bf = q;
        break;
    }

    r = (uint8_t)(rf * 255);
    g = (uint8_t)(gf * 255);
    b = (uint8_t)(bf * 255);
}
