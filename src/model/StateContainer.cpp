#include "hardware/MitsubishiInterface.h"
#include "wifiCredentials.h"
#include "StateContainer.h"

MitsubishiInterface mitsubishiSend;

WiFiClient client;

float setPointStep = 0.25;

void StateContainer::updateMitsubishiInterface()
{
    mitsubishiSend.sendHvacMitsubishi(HvacMode::HVAC_COLD, 23, HvacFanMode::FAN_SPEED_5, HvacVanneMode::VANNE_AUTO, HvacPower::ON);
}

StateContainer::StateContainer() : macAddress_(generateMacAddress()),
                                   haDevice_(macAddress_.c_str()),
                                   mqtt_(client, haDevice_),
                                   temperatureSensor_("temperature"),
                                   humiditySensor_("relative-humidity")
{
}

void StateContainer::configure()
{
    mitsubishiSend.prepare();

    haDevice_.setName("Enviropad");
    haDevice_.setSoftwareVersion("0.12");

    humiditySensor_.setIcon("mdi:water-percent");
    humiditySensor_.setName("Relative Humidity");

    temperatureSensor_.setIcon("mdi:thermometer");
    temperatureSensor_.setName("Temperature");

    Serial.print("Connecting to MQTT\n");

    if (mqtt_.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true)
    {
        Serial.print("Connected to MQTT broker");
    }
    else
    {
        Serial.print("Could not connect to MQTT broker");
    }
}

// getters
StateData StateContainer::getState() { return stateData_; };

// setters
void StateContainer::setTemperature(float temperature)
{
    if (temperature != stateData_.temperature)
    {
        stateData_.temperature = temperature;
        temperatureSensor_.setValue(temperature);
        Serial.print(F("logging temp update"));
    }
}

void StateContainer::setHumidity(float humidity)
{
    if (humidity != stateData_.humidity)
    {
        stateData_.humidity = humidity;
        humiditySensor_.setValue(humidity);
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
    mqtt_.loop();
}