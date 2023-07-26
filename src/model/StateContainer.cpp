#include "hardware/MitsubishiInterface.h"
#include "wifiCredentials.h"
#include "StateContainer.h"

#include <ArduinoHA.h>
#include <WiFi.h>

MitsubishiInterface mitsubishiSend;

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

HASensorNumber humiditySensor("relative-humidity");
HASensorNumber temperatureSensor("temperature");

float setPointStep = 0.25;

void StateContainer::updateMitsubishiInterface()
{
    mitsubishiSend.sendHvacMitsubishi(HvacMode::HVAC_COLD, 23, HvacFanMode::FAN_SPEED_5, HvacVanneMode::VANNE_AUTO, HvacPower::ON);
}

StateContainer::StateContainer() : temperature_(0), humidity_(0), setPoint_(0), hvacMode_(HVACMode::OFF), fanSpeed_(0)
{
}

void StateContainer::configure()
{
    mitsubishiSend.prepare();

    byte mac[32];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    device.setName("Enviropad");
    device.setSoftwareVersion("0.1");

    humiditySensor.setIcon("mdi:water-percent");
    humiditySensor.setName("Relative Humidity");

    temperatureSensor.setIcon("mdi:thermometer");
    temperatureSensor.setName("Temperature");

    Serial.print("Connecting to MQTT\n");

    if (mqtt.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true)
    {
        Serial.print("Connected to MQTT broker");
    }
    else
    {
        Serial.print("Could not connect to MQTT broker");
    }
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
    if (temperature != temperature_)
    {
        temperature_ = temperature;
        temperatureSensor.setCurrentValue(temperature_);
    }
}

void StateContainer::setHumidity(float humidity)
{
    if (humidity != humidity_)
    {
        humidity_ = humidity;
        humiditySensor.setCurrentValue(humidity_);
    }
}

void StateContainer::setSetPoint(float setPoint)
{
    if (setPoint != setPoint_)
    {
        setPoint_ = setPoint;
        updateMitsubishiInterface();
    }
}

void StateContainer::setHVACMode(HVACMode hvacMode)
{
    if (hvacMode != hvacMode_)
    {
        hvacMode_ = hvacMode;
        updateMitsubishiInterface();
    }
}

void StateContainer::setFanSpeed(int fanSpeed)
{
    if (fanSpeed != fanSpeed_)
    {
        fanSpeed_ = fanSpeed;
        updateMitsubishiInterface();
    }
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

void StateContainer::heartbeat()
{
    mqtt.loop();
}