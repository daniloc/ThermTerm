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

StateContainer::StateContainer() : temperature_(0),
                                   humidity_(0),
                                   setPoint_(0),
                                   hvacMode_(HVACMode::OFF),
                                   fanSpeed_(0),
                                   macAddress_(generateMacAddress()),
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
        temperatureSensor_.setValue(temperature_);
        Serial.print(F("logging temp update"));
    }
}

void StateContainer::setHumidity(float humidity)
{
    if (humidity != humidity_)
    {
        humidity_ = humidity;
        humiditySensor_.setValue(humidity_);
        Serial.print(F("logging humidity update"));
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
    mqtt_.loop();
}