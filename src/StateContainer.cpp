#include "StateContainer.h"
#include "hardware/MitsubishiInterface.h"

MitsubishiInterface mitsubishiSend;

float setPointStep = 0.25;

void StateContainer::updateMitsubishiInterface()
{
    mitsubishiSend.sendHvacMitsubishi(HvacMode::HVAC_COLD, 23, HvacFanMode::FAN_SPEED_5, HvacVanneMode::VANNE_AUTO, HvacPower::ON);
}

StateContainer::StateContainer() : temperature_(0), humidity_(0), setPoint_(0), hvacMode_(HVACMode::OFF), fanSpeed_(0) {
    mitsubishiSend.prepare();
}

StateContainer::StateContainer(float temperature, float humidity, float setPoint, HVACMode hvacMode, int fanSpeed)
    : temperature_(temperature), humidity_(humidity), setPoint_(setPoint), hvacMode_(hvacMode), fanSpeed_(fanSpeed) {
    mitsubishiSend.prepare();
    }

// getters
float StateContainer::getTemperature() const { return temperature_; }
float StateContainer::getHumidity() const { return humidity_; }
float StateContainer::getSetPoint() const { return setPoint_; }
HVACMode StateContainer::getHVACMode() const { return hvacMode_; }
int StateContainer::getFanSpeed() const { return fanSpeed_; }

// setters
void StateContainer::setTemperature(float temperature)
{
    temperature_ = temperature;
    updateMitsubishiInterface();
}
void StateContainer::setHumidity(float humidity)
{
    humidity_ = humidity;
    updateMitsubishiInterface();
}
void StateContainer::setSetPoint(float setPoint)
{
    setPoint_ = setPoint;
    updateMitsubishiInterface();
}
void StateContainer::setHVACMode(HVACMode hvacMode)
{
    hvacMode_ = hvacMode;
    updateMitsubishiInterface();
}
void StateContainer::setFanSpeed(int fanSpeed)
{
    fanSpeed_ = fanSpeed;
    updateMitsubishiInterface();
}

void StateContainer::incrementSetPoint()
{
    setPoint_ += setPointStep;
    updateMitsubishiInterface();
}

void StateContainer::decrementSetPoint()
{
    setPoint_ -= setPointStep;
    updateMitsubishiInterface();
}
