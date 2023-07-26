#include "hardware/MitsubishiInterface.h"
#include "StateContainer.h"

MitsubishiInterface mitsubishiSend;

float setPointStep = 0.25;

void StateContainer::updateMitsubishiInterface()
{
    mitsubishiSend.sendHvacMitsubishi(HvacMode::HVAC_COLD, 23, HvacFanMode::FAN_SPEED_5, HvacVanneMode::VANNE_AUTO, HvacPower::ON);
}

StateContainer::StateContainer() : haInterface_(HAInterface())
{
}

void StateContainer::configure()
{
    mitsubishiSend.prepare();
    haInterface_.configure();
}

// getters
StateData StateContainer::getState() { return stateData_; };

// setters
void StateContainer::setTemperature(float temperature)
{
    if (temperature != stateData_.temperature)
    {
        stateData_.temperature = temperature;
        haInterface_.getTemperatureSensor().setValue(temperature);
        Serial.print(F("logging temp update"));
    }
}

void StateContainer::setHumidity(float humidity)
{
    if (humidity != stateData_.humidity)
    {
        stateData_.humidity = humidity;
        haInterface_.getHumiditySensor().setValue(humidity);
        Serial.print(F("logging humidity update"));
    }
}

void StateContainer::setSetPoint(float setPoint)
{
    if (setPoint != stateData_.setPoint)
    {
        stateData_.setPoint = setPoint;
        updateMitsubishiInterface();
    }
}

void StateContainer::setHVACMode(HvacMode hvacMode)
{
    if (hvacMode != stateData_.hvacMode)
    {
        stateData_.hvacMode = hvacMode;
        updateMitsubishiInterface();
    }
}

void StateContainer::setFanSpeed(HvacFanMode fanSpeed)
{
    if (fanSpeed != stateData_.fanSpeed)
    {
        stateData_.fanSpeed = fanSpeed;
        updateMitsubishiInterface();
    }
}

void StateContainer::incrementSetPoint()
{
    stateData_.setPoint += setPointStep;
    updateMitsubishiInterface();
}

void StateContainer::decrementSetPoint()
{
    stateData_.setPoint -= setPointStep;
    updateMitsubishiInterface();
}

void StateContainer::heartbeat()
{
    haInterface_.heartbeat();
}