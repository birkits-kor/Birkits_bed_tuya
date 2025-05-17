#include "IRDecoder.h"
#include <IRremote.hpp>

void IRDecoder::setup()
{
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // 수신기 시작
}
IRCommand IRDecoder::getCommand()
{
    unsigned long now = millis();

    if (IrReceiver.decode())
    {
        uint16_t raw = IrReceiver.decodedIRData.command;
        IrReceiver.resume();
        lastIRReceivedTime = now;

        // 일반 명령 처리
        if (raw == lastRawCommand)
        {
            // 같은 버튼 계속 누르고 있을 때 500ms마다 리턴
            if (now - lastCommandTime >= 500)
            {
                lastCommandTime = now;
                return decode(raw);
            }
            else
            {
                return IRCommand::NONE;
            }
        }
        else
        {
            // 새로운 명령 버튼
            lastRawCommand = raw;
            lastCommandTime = now;
            return decode(raw);
        }
    }

    return IRCommand::NONE;
}

IRCommand IRDecoder::decode(uint16_t raw)
{
    switch (raw)
    {
    case 0x70:
        return IRCommand::STOP;
    case 0x64:
        return IRCommand::BACKREST_UP;
    case 0x23:
        return IRCommand::BACKREST_DOWN;
    case 0x6F:
        return IRCommand::LEG_UP;
    case 0x26:
        return IRCommand::LEG_DOWN;
    case 0x20:
        return IRCommand::TABLE_FORWARD;
    case 0x21:
        return IRCommand::TABLE_BACKWARD;
    case 0x19:
        return IRCommand::SPEAKER;
    case 0x1B:
        return IRCommand::LIGHT;
    case 0x1F:
        return IRCommand::POSE1;
    case 0x11:
        return IRCommand::POSE2;
    case 0x0E:
        return IRCommand::POSE3;
    case 0x12:
        return IRCommand::POSE4;
    case 0x10:
        return IRCommand::POSE5;
    case 0x13:
        return IRCommand::POSE6;
    default:
        return IRCommand::NONE;
    }
}