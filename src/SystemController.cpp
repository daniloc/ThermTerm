#include "hardware/infrared/MitsubishiInterface.h"
#include "hardware/Audio.h"
#include "SystemController.h"
#include "_Constants.h"

#include <functional>

SystemController *SystemController::instance = nullptr; // Initialize static member

float setPointStep = 1;

SystemController::SystemController(IRInterface &irInterface) : haInterface_(HAInterface()),
                                                           irInterface_(irInterface)
{
    instance = this;
}

bool SystemController::shouldDimScreen()
{
    return state_.lux < DIM_SCREEN_CUTOFF_LUX;
}

void SystemController::scheduleUpdate()
{
    needsUpdate = true;
    batchScheduleTime = 0;
}

void SystemController::checkInputBatching()
{
    if (!needsUpdate)
        return;

    if (batchScheduleTime == 0)
    {
        batchScheduleTime = millis();
        return;
    }

    if (millis() - batchScheduleTime >= 2000)
    {
        sendInfraredCommand();
        needsUpdate = false;
        batchScheduleTime = 0;
    }
}

void SystemController::sendInfraredCommand()
{
    irInterface_.sendCommand(state_.hvacCommand());
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
        return FAN_SPEED_4;
    default:
        return FAN_SPEED_AUTO;
    }
}

void SystemController::handleRemoteModeChange(HAHVAC::Mode newMode, HAHVAC *sender)
{
    if (newMode == HAHVAC::Mode::OffMode)
    {
        state_.power = OFF;
    }
    else
    {
        state_.hvacMode = convertMode(newMode);
        state_.power = ON;
    }

    sender->setMode(newMode);
    sendInfraredCommand();
    notifyObservers();
}

void SystemController::handleRemotePowerChange(bool powerState, HAHVAC *sender)
{
    if (powerState)
    {
        state_.power = ON;
        haInterface_.getHVACDevice().setMode(sender->getCurrentMode());
    }
    else
    {
        state_.power = OFF;
        haInterface_.getHVACDevice().setMode(HAHVAC::Mode::OffMode);
    }

    sendInfraredCommand();
    notifyObservers();
}

void SystemController::handleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender)
{
    Serial.print("updating fan mode" + String(fanMode));
    state_.fanSpeed = convertFanMode(fanMode);
    state_.power = ON;
    sender->setFanMode(fanMode);
    sendInfraredCommand();
    notifyObservers();
}

void SystemController::handleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender)
{
    Serial.print("receiving remote temperature change");
    setSetPoint(temperature.toFloat());
}

void SystemController::configure()
{
    irInterface_.configure();
    haInterface_.configure(state_.celsius);

    haInterface_.getHVACDevice().onModeCommand(&SystemController::staticHandleRemoteModeChange);
    haInterface_.getHVACDevice().onPowerCommand(SystemController::staticHandleRemotePowerChange);
    haInterface_.getHVACDevice().onFanModeCommand(SystemController::staticHandleRemoteFanModeChange);
    haInterface_.getHVACDevice().onTargetTemperatureCommand(SystemController::staticHandleRemoteTargetTemperatureChange);

    haInterface_.getAlertTrigger().onCommand(SystemController::staticHandleAlert);
}

// getters
SystemState SystemController::getState() { return state_; };

// setters
void SystemController::setTemperature(float temperature)
{
    if (temperature != state_.temperature)
    {
        state_.temperature = temperature;
        haInterface_.getTemperatureSensor().setValue(temperature);
        Serial.print(F("logging temp update"));
    }
}

void SystemController::setHumidity(float humidity)
{
    if (humidity != state_.humidity)
    {
        state_.humidity = humidity;
        haInterface_.getHumiditySensor().setValue(humidity);
        Serial.print(F("logging humidity update"));
    }
}

void SystemController::setLux(float lux)
{
    if (lux != state_.lux)
    {
        state_.lux = lux;
        haInterface_.getLightSensor().setValue(lux);
        Serial.print(F("Logging light update"));
    }
}

void SystemController::setSetPoint(float setPoint)
{
    if (setPoint != state_.getSetPointUIValue())
    {
        state_.updateSetPoint(setPoint);
        haInterface_.getHVACDevice().setTargetTemperature(setPoint);
        scheduleUpdate();
        notifyObservers();
    }
}

void SystemController::setHVACMode(HvacMode hvacMode)
{

    state_.hvacMode = hvacMode;
    state_.power = ON;
    haInterface_.getHVACDevice().setMode(reverseConvertMode(hvacMode));
    sendInfraredCommand();
    notifyObservers();
}

void SystemController::setFanSpeed(HvacFanMode fanSpeed)
{

    state_.fanSpeed = fanSpeed;
    scheduleUpdate();
}

void SystemController::incrementSetPoint()
{
    float setPoint = state_.getSetPointUIValue();
    setPoint += setPointStep;
    setSetPoint(setPoint);
    Audio::play_UpwardTone();
}

void SystemController::decrementSetPoint()
{
    float setPoint = state_.getSetPointUIValue();
    setPoint -= setPointStep;
    setSetPoint(setPoint);
    Audio::play_DownwardTone();
}

void SystemController::togglePower()
{

    if (state_.power == ON)
    {
        state_.power = OFF;
        haInterface_.getHVACDevice().setMode(HAHVAC::Mode::OffMode);
    }
    else
    {
        state_.power = ON;
        haInterface_.getHVACDevice().setMode(reverseConvertMode(state_.hvacMode));
    }

    needsUpdate = false;
    sendInfraredCommand();
    notifyObservers();
}

void SystemController::heartbeat()
{
    haInterface_.heartbeat();
    checkInputBatching();
}

// Static->Instance bridging
void SystemController::staticHandleRemotePowerChange(bool powerState, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemotePowerChange(powerState, sender);
    }
}

void SystemController::staticHandleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteFanModeChange(fanMode, sender);
    }
}

void SystemController::staticHandleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteTargetTemperatureChange(temperature, sender);
    }
}
void SystemController::staticHandleRemoteModeChange(HAHVAC::Mode mode, HAHVAC *sender)
{
    if (instance)
    {
        instance->handleRemoteModeChange(mode, sender);
    }
}

void SystemController::staticHandleAlert(bool status, HASwitch *sender)
{
    Serial.print("switch changed");
    sender->setState(status);
}