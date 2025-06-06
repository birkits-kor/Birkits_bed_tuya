#include "LedController.h"
#include "../storage/NVSStorage.h"

LedController *LedController::instance = nullptr;

LedController::LedController()
    : _strip1(LED_COUNT, LED_PIN1, NEO_RBG + NEO_KHZ800),
      _strip2(LED_COUNT, LED_PIN2, NEO_RBG + NEO_KHZ800)
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
    hsvToRgb(lightControlData.h, lightControlData.s, lightControlData.v, _r, _g, _b);
}

void LedController::set(uint16_t h, uint16_t s, uint16_t v, uint8_t mode,
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

void LedController::getLightControlData(uint16_t &h, uint16_t &s, uint16_t &v,
                                        String &endTime,
                                        String &startTime,
                                        String &mode,
                                        bool &sw) const
{
    h = lightControlData.h;
    s = lightControlData.s;
    v = lightControlData.v;
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
    // 모드 2: 디밍 (2초 주기, 50ms * 40단계)
    uint8_t phase = (_count % 40); // 0~39
    uint8_t brightness;

    if (phase < 20)
        brightness = phase * 5;        // 0 → 95
    else
        brightness = (40 - phase) * 5; // 100 → 5

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
    uint16_t hue = (_count % 100) * 655; // 0~65500까지 5초간 변화
    uint32_t color = _strip1.ColorHSV(hue);
    for (int i = 0; i < LED_COUNT; ++i)
    {
        _strip1.setPixelColor(i, color);
        _strip2.setPixelColor(i, color);
    }
    _strip1.show();
    _strip2.show();
}

void LedController::mode4()
{
    const uint8_t steps = 30;
    const uint16_t hueStep = 65535 / steps;

    for (int i = 0; i < LED_COUNT; ++i)
    {
        uint16_t hueIndex = (i + _count) % steps;
        uint16_t hue = hueIndex * hueStep;
        uint32_t color = _strip1.ColorHSV(hue);

        _strip1.setPixelColor(i, color);
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
        lightControlData.v = color[2];

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
            50,
            "14:00",
            "06:00",
            "none",
            false};
    }
}

void LedController::saveData(String data)
{
    String lightDataStr = NVSStorage::getInstance().getCredential("light_control");
    if (data == lightDataStr)
        return;

    NVSStorage::getInstance().saveCredential("light_control", data);
    loadSavedData();
    hsvToRgb(lightControlData.h, lightControlData.s, lightControlData.v, _r, _g, _b);
}

void LedController::hsvToRgb(uint16_t h, uint16_t s, uint16_t v, uint8_t &r, uint8_t &g, uint8_t &b)
{
      h = h % 360; // <-- 반드시 래핑!

    float H = h / 60.0f;
    float S = s / 1000.0f;
    float V = v / 100.0f;

    int i = int(H);
    float f = H - i;

    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    float R, G, B;

    switch (i)
    {
    case 0: R = V; G = t; B = p; break;
    case 1: R = q; G = V; B = p; break;
    case 2: R = p; G = V; B = t; break;
    case 3: R = p; G = q; B = V; break;
    case 4: R = t; G = p; B = V; break;
    case 5: R = V; G = p; B = q; break;
    default: R = G = B = 0; break;
    }

    r = (uint8_t)(R * 255 + 0.5f);
    g = (uint8_t)(G * 255 + 0.5f);
    b = (uint8_t)(B * 255 + 0.5f);
}

bool LedController::getSw()
{
    return lightControlData.light_switch;
}

bool LedController::isWithinLightTime(int hour, int minute) {
    // 시간 문자열 가져오기
    String startStr = lightControlData.light_start_time;
    String endStr = lightControlData.light_end_time;

    // ":" 기준으로 안전하게 분리
    int startColon = startStr.indexOf(':');
    int endColon = endStr.indexOf(':');

    if (startColon == -1 || endColon == -1) {
        // 잘못된 형식의 시간 문자열
        return false;
    }

    int startHour = startStr.substring(0, startColon).toInt();
    int startMinute = startStr.substring(startColon + 1).toInt();

    int endHour = endStr.substring(0, endColon).toInt();
    int endMinute = endStr.substring(endColon + 1).toInt();

    // 분 단위로 환산
    int currentTotal = hour * 60 + minute;
    int startTotal = startHour * 60 + startMinute;
    int endTotal = endHour * 60 + endMinute;

    if (startTotal <= endTotal) {
        // 자정을 넘지 않는 범위 (예: 16:00 ~ 23:00)
        return currentTotal >= startTotal && currentTotal <= endTotal;
    } else {
        // 자정을 넘는 범위 (예: 22:00 ~ 06:00)
        return currentTotal >= startTotal || currentTotal <= endTotal;
    }
}