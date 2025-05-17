#ifndef IRDECODER_H
#define IRDECODER_H

#include <Arduino.h>

#define IR_RECEIVE_PIN 32 // 사용 중인 IR 수신 핀 (예: D15)

// 명령 Enum - 기본 커맨드 + 포즈(자세) SHORT/LONG
enum class IRCommand {
    NONE,
    STOP,
    BACKREST_UP,
    BACKREST_DOWN,
    LEG_UP,
    LEG_DOWN,
    TABLE_FORWARD,
    TABLE_BACKWARD,
    POSE1,
    POSE2,
    POSE3,
    POSE4,
    POSE5,
    POSE6,
    SPEAKER,
    LIGHT
};

class IRDecoder {
public:
    IRCommand getCommand();
    void setup();

private:
    IRCommand decode(uint16_t raw);

    uint16_t lastRawCommand = 0;
    unsigned long lastCommandTime = 0;

    // 포즈 커맨드 상태 추적용
    bool posePressed = false;
    uint16_t poseRawCommand = 0;
    unsigned long posePressStartTime = 0;
    bool poseLongTriggered = false;

    unsigned long lastIRReceivedTime = 0;
};

#endif // IRDECODER_H