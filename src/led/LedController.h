#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#define LED_PIN1 21
#define LED_PIN2 33
#define LED_COUNT 72

struct LightControlData
{
    int h, s;
    double l;                // RGB 또는 HSL 등 색상 3값
    String light_end_time;   // "06:00"
    String light_start_time; // "16:00"
    String light_mode;       // "mode_a"
    bool light_switch;       // true / false
};

class LedController
{
public:
    static LedController *getInstance();

    void setup();
    void set(uint16_t h, uint16_t s, uint8_t v, uint8_t mode,
             uint8_t startHour, uint8_t startMinute,
             uint8_t endHour, uint8_t endMinute);
    void reset();                                                  // count 초기화
    void stop();                                                   // 전체 LED 꺼짐
    void run();                                                    // 주기적으로 호출
    void getLightControlData(int &h, int &s, double &l,
                                      String &endTime,
                                      String &startTime,
                                      String &mode,
                                      bool &sw) const;

private:
    LedController();

    void loadSavedData();
    void saveData();
    void hsvToRgb(uint16_t h, uint16_t s, double l, uint8_t &r, uint8_t &g, uint8_t &b);

    void mode1();
    void mode2();
    void mode3();
    void mode4();

    Adafruit_NeoPixel _strip1;
    Adafruit_NeoPixel _strip2;

    LightControlData lightControlData;
    uint8_t _r = 255, _g = 255, _b = 255;

    unsigned long _count = 0;

    static LedController *instance;
};

#endif
