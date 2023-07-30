#include "HAInterface.h"
#include "wifiCredentials.h"

WiFiClient client;

HAInterface::HAInterface() : macAddress_(generateMacAddress()),
                             haDevice_(macAddress_.c_str()),
                             mqtt_(client, haDevice_),
                             temperatureSensor_("temperature"),
                             humiditySensor_("relative-humidity"),
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
    haDevice_.setName("Enviropad");
    haDevice_.setSoftwareVersion("0.12");

    humiditySensor_.setIcon("mdi:water-percent");
    humiditySensor_.setName("Relative Humidity");

    temperatureSensor_.setIcon("mdi:thermometer");
    temperatureSensor_.setName("Temperature");

    hvacDevice_.setName("Heat Pump");
    hvacDevice_.setCurrentMode(HAHVAC::CoolMode);
    hvacDevice_.setFanModes(HAHVAC::AutoFanMode |
    HAHVAC::LowFanMode |
     HAHVAC::MediumFanMode |
     HAHVAC::HighFanMode);

    if (useCelsius) {
        hvacDevice_.setTemperatureUnit(HAHVAC::TemperatureUnit::CelsiusUnit);
    } else {
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