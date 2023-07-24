#include "StateContainer.h"

float setPointStep = 0.25;

StateContainer::StateContainer() : temperature_(0), humidity_(0), setPoint_(0), hvacMode_(HVACMode::OFF), fanSpeed_(0) {}

StateContainer::StateContainer(float temperature, float humidity, float setPoint, HVACMode hvacMode, int fanSpeed)
    : temperature_(temperature), humidity_(humidity), setPoint_(setPoint), hvacMode_(hvacMode), fanSpeed_(fanSpeed) {}

// getters
float StateContainer::getTemperature() const { return temperature_; }
float StateContainer::getHumidity() const { return humidity_; }
float StateContainer::getSetPoint() const { return setPoint_; }
HVACMode StateContainer::getHVACMode() const { return hvacMode_; }
int StateContainer::getFanSpeed() const { return fanSpeed_; }

// setters
void StateContainer::setTemperature(float temperature) { temperature_ = temperature; }
void StateContainer::setHumidity(float humidity) { humidity_ = humidity; }
void StateContainer::setSetPoint(float setPoint) { setPoint_ = setPoint; }
void StateContainer::setHVACMode(HVACMode hvacMode) { hvacMode_ = hvacMode; }
void StateContainer::setFanSpeed(int fanSpeed) { fanSpeed_ = fanSpeed; }

void StateContainer::incrementSetPoint() {
    setPoint_+= setPointStep;
}

void StateContainer::decrementSetPoint() {
    setPoint_ -= setPointStep;
}
