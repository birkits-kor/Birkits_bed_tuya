#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN1 21
#define LED_PIN2 33
#define LED_COUNT 72

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
    void get(uint16_t &h, uint16_t &s, uint8_t &v, uint8_t &mode, uint8_t &startHour, uint8_t &startMin, uint8_t &endHour, uint8_t &endMin); // HSV와 mode 개별값 반환

private:
    LedController();

    void loadSavedData();
    void saveData();
    void hsvToRgb(uint16_t h, uint16_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b);

    void mode1();
    void mode2();
    void mode3();
    void mode4();

    Adafruit_NeoPixel _strip1;
    Adafruit_NeoPixel _strip2;

    uint16_t _h = 0;
    uint16_t _s = 0;  // 0 ~ 1000
    uint8_t _v = 100; // 0 ~ 100
    uint8_t _mode = 1;
    uint8_t _startHour;   // 0~23
    uint8_t _startMinute; // 0~59
    uint8_t _endHour;     // 0~23
    uint8_t _endMinute;   // 0~59
    uint8_t _r = 255, _g = 255, _b = 255;

    unsigned long _count = 0;

    static LedController *instance;
};

#endif
