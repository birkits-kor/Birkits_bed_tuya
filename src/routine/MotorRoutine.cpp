#include "MotorRoutine.h"

MotorRoutine::MotorRoutine()
{
}

// void MotorRoutine::addMotor(const String &motorName, MotorController *motor)
// {
//     motors[motorName] = motor;
// }

// void MotorRoutine::moveForward(const String &motorName) {
//     if (motors.find(motorName) != motors.end()) {
//         motors[motorName]->moveTo(50);
//     }
// }

// void MotorRoutine::moveBackward(const String &motorName) {
//     if (motors.find(motorName) != motors.end()) {
//         motors[motorName]->moveBackward();
//     }
// }

// void MotorRoutine::stopMotor(const String &motorName) {
//     if (motors.find(motorName) != motors.end()) {
//         motors[motorName]->stopMotor();
//     }
// }

int MotorRoutine::getSensorValue(const String &motorName) {
    return BackrestMotorController::getInstance()->getPosition();
}

// void MotorRoutine::stopAllMotors() {
//     for (auto &motorPair : motors) {
//         motorPair.second->stopMotor();
//     }
// }

void MotorRoutine::begin()
{
    BackrestMotorController::getInstance()->setupMotor();
}
