#include "SpeakerController.h"

SpeakerController* SpeakerController::instance = nullptr;

SpeakerController::SpeakerController() {
    pinMode(SPEAKER_PIN, OUTPUT);
    digitalWrite(SPEAKER_PIN, LOW);
    _state = false;
}

SpeakerController* SpeakerController::getInstance() {
    if (instance == nullptr)
        instance = new SpeakerController();
    return instance;
}

void SpeakerController::on() {
    digitalWrite(SPEAKER_PIN, HIGH);
    _state = true;
}

void SpeakerController::off() {
    digitalWrite(SPEAKER_PIN, LOW);
    _state = false;
}

bool SpeakerController::isOn() const {
    return _state;
}
