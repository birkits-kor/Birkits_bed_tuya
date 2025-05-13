#ifndef SPEAKER_CONTROLLER_H
#define SPEAKER_CONTROLLER_H

#include <Arduino.h>

#define SPEAKER_PIN 4  // 스피커 제어 핀

class SpeakerController {
public:
    static SpeakerController* getInstance();

    void on();                  // 스피커 켜기
    void off();                 // 스피커 끄기
    bool isOn() const;          // 스피커 상태 확인

private:
    SpeakerController();        // 생성자에서 핀 초기화
    static SpeakerController* instance;
    bool _state = false;        // 현재 상태 저장 (true: ON, false: OFF)
};

#endif
