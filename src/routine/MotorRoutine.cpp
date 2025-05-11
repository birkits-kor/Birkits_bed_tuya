#include "MotorRoutine.h"

MotorRoutine::MotorRoutine() {
    MotorController* backrestMotor = new MotorController(BACKREST_MOTOR_PIN1, BACKREST_MOTOR_PIN2, BACKREST_HALL_SENSOR_PIN);
    MotorController* legsMotor = new MotorController(LEGS_MOTOR_PIN1, LEGS_MOTOR_PIN2, LEGS_HALL_SENSOR_PIN);
    MotorController* tableMotor = new MotorController(TABLE_MOTOR_PIN1, TABLE_MOTOR_PIN2, TABLE_HALL_SENSOR_PIN);
    
    addMotor("backrest", backrestMotor);
    addMotor("legs", legsMotor);
    addMotor("table", tableMotor);
    
    begin();
}

void MotorRoutine::addMotor(const String &motorName, MotorController *motor) {
    motors[motorName] = motor;
}

void MotorRoutine::moveForward(const String &motorName) {
    if (motors.find(motorName) != motors.end()) {
        motors[motorName]->moveForward();
    }
}

void MotorRoutine::moveBackward(const String &motorName) {
    if (motors.find(motorName) != motors.end()) {
        motors[motorName]->moveBackward();
    }
}

void MotorRoutine::stopMotor(const String &motorName) {
    if (motors.find(motorName) != motors.end()) {
        motors[motorName]->stopMotor();
    }
}

int MotorRoutine::getSensorValue(const String &motorName) {
    if (motors.find(motorName) != motors.end()) {
        return motors[motorName]->getSensorValue();
    }
    return -1; // Return -1 if motor is not found
}

void MotorRoutine::stopAllMotors() {
    for (auto &motorPair : motors) {
        motorPair.second->stopMotor();
    }
}

void MotorRoutine::begin() {
    for (auto &motorPair : motors) {
        motorPair.second->begin();
    }
}
