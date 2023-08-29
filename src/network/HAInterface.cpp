#include "HAInterface.h"
#include "wifiCredentials.h"
#include "_Constants.h"

WiFiClient client;

HAInterface::HAInterface() : macAddress_(generateMacAddress()),
                             haDevice_(macAddress_.c_str()),
                             mqtt_(client, haDevice_),
                             temperatureSensor_("temperature"),
                             humiditySensor_("relative-humidity"),
                             lightSensor_("light-intensity"),
                             alertTrigger_("alert-trigger"),
                             hvacDevice_(
                                 "heat-pump",
                                 HAHVAC::TargetTemperatureFeature |
                                     HAHVAC::PowerFeature |
                                     HAHVAC::ModesFeature |
                                     HAHVAC::FanFeature)
{
}

void HAInterface::configure(bool useCelsius)
{
    haDevice_.setName("Thermterm");
    haDevice_.setSoftwareVersion(VERSION_NUMBER);

    humiditySensor_.setIcon("mdi:water-percent");
    humiditySensor_.setName("Relative Humidity");
    humiditySensor_.setUnitOfMeasurement("%");

    temperatureSensor_.setIcon("mdi:thermometer");
    temperatureSensor_.setName("Temperature");
    temperatureSensor_.setUnitOfMeasurement("°");

    lightSensor_.setIcon("mdi:brightness-5");
    lightSensor_.setName("Light");
    lightSensor_.setUnitOfMeasurement("lux");

    alertTrigger_.setName("Alert");

    hvacDevice_.setName("Heat Pump");
    hvacDevice_.setCurrentMode(HAHVAC::CoolMode);
    hvacDevice_.setFanModes(HAHVAC::AutoFanMode |
                            HAHVAC::LowFanMode |
                            HAHVAC::MediumFanMode |
                            HAHVAC::HighFanMode);

    if (useCelsius)
    {
        hvacDevice_.setTemperatureUnit(HAHVAC::TemperatureUnit::CelsiusUnit);
    }
    else
    {
        hvacDevice_.setTemperatureUnit(HAHVAC::TemperatureUnit::FahrenheitUnit);
    }

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

void HAInterface::heartbeat()
{
    mqtt_.loop();
}