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
    hsvToRgb(_h, _s, _v, _r, _g, _b);
}

void LedController::set(uint16_t h, uint16_t s, uint8_t v, uint8_t mode,
                        uint8_t startHour, uint8_t startMinute,
                        uint8_t endHour, uint8_t endMinute)
{
    _h = h;
    _s = s;
    _v = v;
    _mode = mode;
    _startHour = startHour;
    _startMinute = startMinute;
    _endHour = endHour;
    _endMinute = endMinute;

    hsvToRgb(_h, _s, _v, _r, _g, _b);
    saveData();
    reset();
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

void LedController::get(uint16_t &h, uint16_t &s, uint8_t &v, uint8_t &mode, uint8_t &startHour, uint8_t &startMin, uint8_t &endHour, uint8_t &endMin)
{
    h = _h;
    s = _s;
    v = _v;
    mode = _mode;

    startHour = _startHour;
    startMin = _startMinute;
    endHour = _endHour;
    endMin = _endMinute;
}

void LedController::run()
{
    ++_count;

    switch (_mode)
    {
    case 1:
        mode1();
        break;

    case 2:
        mode2();
        break;

    case 3:
        mode3();
        break;

    case 4:
        mode4();
        break;
    }
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
    hsvToRgb(_h, _s, brightness, r, g, b);
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
    String val = NVSStorage::getInstance().getCredential("led_config");
    if (val.length() == 0)
    {
        _h = 0;
        _s = 0;
        _v = 100;
        _mode = 1;
        _startHour = 0;
        _startMinute = 0;
        _endHour = 23;
        _endMinute = 59;
        saveData();
        return;
    }

    int sep1 = val.indexOf(',');
    int sep2 = val.indexOf(',', sep1 + 1);
    int sep3 = val.indexOf(',', sep2 + 1);
    int sep4 = val.indexOf(',', sep3 + 1);
    int sep5 = val.indexOf(',', sep4 + 1);
    int sep6 = val.indexOf(',', sep5 + 1);
    int sep7 = val.indexOf(',', sep6 + 1);

    _h = val.substring(0, sep1).toInt();
    _s = val.substring(sep1 + 1, sep2).toInt();
    _v = val.substring(sep2 + 1, sep3).toInt();
    _mode = val.substring(sep3 + 1, sep4).toInt();
    _startHour = val.substring(sep4 + 1, sep5).toInt();
    _startMinute = val.substring(sep5 + 1, sep6).toInt();
    _endHour = val.substring(sep6 + 1, sep7).toInt();
    _endMinute = val.substring(sep7 + 1).toInt();
}

void LedController::saveData()
{
    String val = String(_h) + "," +
                 String(_s) + "," +
                 String(_v) + "," +
                 String(_mode) + "," +
                 String(_startHour) + "," +
                 String(_startMinute) + "," +
                 String(_endHour) + "," +
                 String(_endMinute);

    NVSStorage::getInstance().saveCredential("led_config", val);
}

void LedController::hsvToRgb(uint16_t h, uint16_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b)
{
    float hf = (float)h / 360.0f;
    float sf = (float)s / 1000.0f;
    float vf = (float)v / 100.0f;

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
