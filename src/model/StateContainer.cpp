#include "hardware/MitsubishiInterface.h"
#include "StateContainer.h"

#include <functional>

StateContainer *StateContainer::instance = nullptr; // Initialize static member

MitsubishiInterface mitsubishiSend;

float setPointStep = 0.25;

void StateContainer::updateMitsubishiInterface()
{
    mitsubishiSend.sendHvacMitsubishi(stateData_.hvacMode, stateData_.heatPumpSetPoint(), stateData_.fanSpeed, HvacVanneMode::VANNE_AUTO, stateData_.power);
}

StateContainer::StateContainer() : haInterface_(HAInterface())
{
    instance = this;
}

HvacMode convertMode(HAHVAC::Mode mode)
{
    switch (mode)
    {
    case HAHVAC::AutoMode:
        return HVAC_AUTO;
    case HAHVAC::CoolMode:
        return HVAC_COLD;
    case HAHVAC::HeatMode:
        return HVAC_HOT;
    case HAHVAC::DryMode:
        return HVAC_DRY;
    case HAHVAC::FanOnlyMode:
        return HVAC_FAN;
    default:
        return HVAC_AUTO; // default value
    }
}

HAHVAC::Mode reverseConvertMode(HvacMode mode)
{
    switch (mode)
    {
    case HVAC_AUTO:
        return HAHVAC::AutoMode;
    case HVAC_COLD:
        return HAHVAC::CoolMode;
    case HVAC_HOT:
        return HAHVAC::HeatMode;
    case HVAC_DRY:
        return HAHVAC::DryMode;
    case HVAC_FAN:
        return HAHVAC::FanOnlyMode;
    default:
        return HAHVAC::AutoMode; // default value
    }
}

HvacFanMode convertFanMode(HAHVAC::FanMode haFanMode)
{
    switch (haFanMode)
    {
    case HAHVAC::AutoFanMode:
        return FAN_SPEED_AUTO;
    case HAHVAC::LowFanMode:
        return FAN_SPEED_1;
    case HAHVAC::MediumFanMode:
        return FAN_SPEED_3;
    case HAHVAC::HighFanMode:
        return FAN_SPEED_5;
    default:
        return FAN_SPEED_AUTO;
    }
}

void StateContainer::handleRemoteModeChange(HAHVAC::Mode newMode, HAHVAC *sender)
{
    if (newMode == HAHVAC::Mode::OffMode) {
        stateData_.power = OFF;
    } else {
        stateData_.hvacMode = convertMode(newMode);
        stateData_.power = ON;
    }

    sender->setMode(newMode);
    updateMitsubishiInterface();
    notifyObservers();
}

void StateContainer::handleRemotePowerChange(bool powerState, HAHVAC *sender)
{
    if (powerState) {
        stateData_.power = ON;
        haInterface_.getHVACDevice().setMode(sender->getCurrentMode());
    } else {
        stateData_.power = OFF;
        haInterface_.getHVACDevice().setMode(HAHVAC::Mode::OffMode);
    }

    updateMitsubishiInterface();
    notifyObservers();
}

void StateContainer::handleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender)
{
    Serial.print("updating fan mode" + String(fanMode));
    stateData_.fanSpeed = convertFanMode(fanMode);
    stateData_.power = ON;
    sender->setFanMode(fanMode);
    updateMitsubishiInterface();
    notifyObservers();
}

void StateContainer::handleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender)
{
    Serial.print("receiving remote temperature change");
    setSetPoint(temperature.toFloat());
}

void StateContainer::configure()
{
    mitsubishiSend.prepare();
    haInterface_.configure(stateData_.celsius);

    haInterface_.getHVACDevice().onModeCommand(&StateContainer::staticHandleRemoteModeChange);
    haInterface_.getHVACDevice().onPowerCommand(StateContainer::staticHandleRemotePowerChange);
    haInterface_.getHVACDevice().onFanModeCommand(StateContainer::staticHandleRemoteFanModeChange);
    haInterface_.getHVACDevice().onTargetTemperatureCommand(StateContainer::staticHandleRemoteTargetTemperatureChange);
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

void StateContainer::setLux(float lux) {
    if (lux != stateData_.lux) {
        stateData_.lux = lux;
        haInterface_.getLightSensor().setValue(lux);
        Serial.print(F("Logging light update"));
    }
}

void StateContainer::setSetPoint(float setPoint)
{
    if (setPoint != stateData_.setPoint)
    {
        stateData_.setPoint = setPoint;
        haInterface_.getHVACDevice().setTargetTemperature(setPoint);
        updateMitsubishiInterface();
        notifyObservers();
    }
}

void StateContainer::setHVACMode(HvacMode hvacMode)
{

        stateData_.hvacMode = hvacMode;
        stateData_.power = ON;
        haInterface_.getHVACDevice().setMode(reverseConvertMode(hvacMode));
        updateMitsubishiInterface();
        notifyObservers();
}

void StateContainer::setFanSpeed(HvacFanMode fanSpeed)
{

        stateData_.fanSpeed = fanSpeed;
        updateMitsubishiInterface();
}

void StateContainer::incrementSetPoint()
{
    float setPoint = stateData_.setPoint;
    setPoint += setPointStep;
    setSetPoint(setPoint);
}

void StateContainer::decrementSetPoint()
{
    float setPoint = stateData_.setPoint;
    setPoint -= setPointStep;
    setSetPoint(setPoint);
}

void StateContainer::togglePower() {

    if (stateData_.power == ON) {
        stateData_.power = OFF;
        haInterface_.getHVACDevice().setMode(HAHVAC::Mode::OffMode);
    } else {
        stateData_.power = ON;
        haInterface_.getHVACDevice().setMode(reverseConvertMode(stateData_.hvacMode));
    }

    updateMitsubishiInterface();
    notifyObservers();
}

void StateContainer::heartbeat()
{
    haInterface_.heartbeat();
}

//Static->Instance bridging
void StateContainer::staticHandleRemotePowerChange(bool powerState, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemotePowerChange(powerState, sender);
    }
}

void StateContainer::staticHandleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteFanModeChange(fanMode, sender);
    }
}

void StateContainer::staticHandleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteTargetTemperatureChange(temperature, sender);
    }
}
void StateContainer::staticHandleRemoteModeChange(HAHVAC::Mode mode, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteModeChange(mode, sender);
    }
}