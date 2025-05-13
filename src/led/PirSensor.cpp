#include "PirSensor.h"
#include <Arduino.h>

PirSensor::PirSensor()
{
    pinMode(PIR_SENSOR_PIN1, INPUT);
    pinMode(PIR_SENSOR_PIN2, INPUT);
}

PirSensor& PirSensor::getInstance()
{
    static PirSensor instance;
    return instance;
}

bool PirSensor::get()
{
    return digitalRead(PIR_SENSOR_PIN1) == HIGH || digitalRead(PIR_SENSOR_PIN2) == HIGH;
}
