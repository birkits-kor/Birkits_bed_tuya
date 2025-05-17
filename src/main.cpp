// #include <Arduino.h>
// #include "MainRoutine.h"

// MainRoutine mainRoutine;
// void keyboardTest(char c);

// void setup()
// {
//     mainRoutine.init();
// }

// void loop()
// {
//     mainRoutine.do1msTasks();
//     mainRoutine.do10msTasks();
//     mainRoutine.do100msTasks();

//     if (Serial.available() > 0)
//         keyboardTest(Serial.read());
// }

// void keyboardTest(char c)
// {
//     if (c == 'q')
//         BackrestMotorController::getInstance()->moveUp(50);
//     if (c == 'a')
//         BackrestMotorController::getInstance()->moveDown(50);
//     if (c == 'w')
//         LegrestMotorController::getInstance()->moveUp(50);
//     if (c == 's')
//         LegrestMotorController::getInstance()->moveDown(50);
//     if (c == 'e')
//         TableMotorController::getInstance()->moveUp(50);
//     if (c == 'd')
//         TableMotorController::getInstance()->moveDown(50);
//     if (c == 'z')
//         SpeakerController::getInstance()->on();
//     if (c == 'x')
//         SpeakerController::getInstance()->off();
     
// }



#include <Arduino.h>
#include "communication/IRDecoder.h"

IRDecoder irDecoder;

void setup() {
    Serial.begin(115200);

    // IR 수신기 초기화 (핀 번호는 15번 예시, 필요시 변경)
    irDecoder.setup();  
    Serial.println("IR Receiver Ready");
}

const char* commandToString(IRCommand cmd) {
    switch (cmd) {
        case IRCommand::STOP: return "STOP";
        case IRCommand::BACKREST_UP: return "BACKREST_UP";
        case IRCommand::BACKREST_DOWN: return "BACKREST_DOWN";
        case IRCommand::LEG_UP: return "LEG_UP";
        case IRCommand::LEG_DOWN: return "LEG_DOWN";
        case IRCommand::TABLE_FORWARD: return "TABLE_FORWARD";
        case IRCommand::TABLE_BACKWARD: return "TABLE_BACKWARD";
        case IRCommand::POSE1: return "POSE1_SHORT";
        case IRCommand::POSE2: return "POSE2_SHORT";
        case IRCommand::POSE3: return "POSE3_SHORT";
        case IRCommand::POSE4: return "POSE4_SHORT";
        case IRCommand::POSE5: return "POSE5_SHORT";
        case IRCommand::POSE6: return "POSE6_SHORT";
        case IRCommand::SPEAKER: return "SPEAKER";
        case IRCommand::LIGHT: return "LIGHT";
        default: return "NONE";
    }
}

void loop() {
    IRCommand cmd = irDecoder.getCommand();
    if (cmd != IRCommand::NONE) {
        Serial.print("Received Command: ");
        Serial.println(commandToString(cmd));
    }
}