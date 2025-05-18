#include "MotorRoutine.h"

MotorRoutine::MotorRoutine()
{
}

void MotorRoutine::begin()
{
    BackrestMotorController::getInstance()->setupMotor();
    LegrestMotorController::getInstance()->setupMotor();
    TableMotorController::getInstance()->setupMotor();
    SpeakerController::getInstance();
    irDecoder.setup();
    LedController::getInstance()->stop();
}

void MotorRoutine::updatePos()
{
    BackrestMotorController::getInstance()->updatePos();
    LegrestMotorController::getInstance()->updatePos();
    TableMotorController::getInstance()->updatePos();
}

void MotorRoutine::loopByIr()
{
    if (state != MotorWaitState::NONE)
    {
        switch (state)
        {
        case MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_UP:
            if (TableMotorController::getInstance()->getPosition() < 30)
            {
                TableMotorController::getInstance()->moveTo(TABLE_MAX);
            }
            break;
        }
    }

    IRCommand cmd = irDecoder.getCommand();
    switch (cmd)
    {
    case IRCommand::STOP:
        preCmd = cmd;
        BackrestMotorController::getInstance()->stopMotor();
        LegrestMotorController::getInstance()->stopMotor();
        TableMotorController::getInstance()->stopMotor();
        break;

    case IRCommand::BACKREST_UP:
        if (preCmd != IRCommand::BACKREST_UP)
        {
            BackrestMotorController::getInstance()->moveTo(BACKREST_MAX);
            preCmd = cmd;
        }
        else
        {
            BackrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::BACKREST_DOWN:
        if (preCmd != IRCommand::BACKREST_DOWN)
        {
            BackrestMotorController::getInstance()->moveTo(0);
            preCmd = cmd;
        }
        else
        {
            BackrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::LEG_UP:
        if (preCmd != IRCommand::LEG_UP)
        {
            auto p = TableMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_UP;
                TableMotorController::getInstance()->moveTo(0);
            }
            else
            {
                LegrestMotorController::getInstance()->moveTo(LEGREST_MAX);
            }
            preCmd = cmd;
        }
        else
        {
            LegrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::LEG_DOWN:
        if (preCmd != IRCommand::LEG_DOWN)
        {
            auto p = TableMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_TABLE_ORIGIN_FOR_LEG_DOWN;
                TableMotorController::getInstance()->moveTo(0);
            }
            else
            {
                LegrestMotorController::getInstance()->moveTo(0);
            }
            preCmd = cmd;
        }
        else
        {
            LegrestMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::TABLE_FORWARD:
        if (preCmd != IRCommand::TABLE_FORWARD)
        {
            auto p = LegrestMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_FORWARD;
                LegrestMotorController::getInstance()->moveTo(0);
            }
            else
            {
                TableMotorController::getInstance()->moveTo(TABLE_MAX);
            }
            preCmd = cmd;
        }
        else
        {
            TableMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }
        break;

    case IRCommand::TABLE_BACKWARD:
        if (preCmd != IRCommand::TABLE_BACKWARD)
        {
            auto p = LegrestMotorController::getInstance()->getPosition();
            if (p > 30) // 바로 움직이지 못하는 상태
            {
                state = MotorWaitState::WAIT_LEG_ORIGIN_FOR_TABLE_BACKWARD;
                LegrestMotorController::getInstance()->moveTo(0);
            }
            else
            {
                TableMotorController::getInstance()->moveTo(0);
            }
            preCmd = cmd;
        }
        else
        {
            TableMotorController::getInstance()->stopMotor();
            preCmd = IRCommand::NONE;
        }

    case IRCommand::POSE1:
        // TODO: add handling
        break;

    case IRCommand::POSE2:
        // TODO: add handling
        break;

    case IRCommand::POSE3:
        // TODO: add handling
        break;

    case IRCommand::POSE4:
        // TODO: add handling
        break;

    case IRCommand::POSE5:
        // TODO: add handling
        break;

    case IRCommand::POSE6:
        // TODO: add handling
        break;

    case IRCommand::SPEAKER:
        if (SpeakerController::getInstance()->isOn())
            SpeakerController::getInstance()->off();
        else
            SpeakerController::getInstance()->on();
        break;

    case IRCommand::LIGHT:
        if (!ledState)
        {
            LedController::getInstance()->run();
            ledState =true;
        }
        else
        {
            LedController::getInstance()->stop();
            ledState = false;
        }
        break;

    case IRCommand::NONE:
    default:
        break;
    }
}
