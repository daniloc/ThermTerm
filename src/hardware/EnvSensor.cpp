#include "EnvSensor.h"
#include "Adafruit_SHTC3.h"
#include <hp_BH1750.h> //  include the library
#include <arduino-timer.h>

Adafruit_SHTC3 shtc3;
hp_BH1750 BH1750;

auto envSensorTimer = timer_create_default();

#define ENV_SENSOR_INTERVAL 5 // Seconds

EnvSensorCallback EnvSensor::envDataUpdateFunction = nullptr;

bool EnvSensor::timerEvent(void *argument)
{

    // Update sensor data
    EnvData envData = update();

    // If a callback function has been set, call it with the updated data
    if (envDataUpdateFunction != nullptr)
    {
        envDataUpdateFunction(envData.temp, envData.humidity, envData.lux);
    }

    return true;
}

void EnvSensor::configure()
{
    if (!shtc3.begin())
    {
        Serial.println("Couldn't find SHTC3");
    }
    else
    {
        Serial.println("Found SHTC3 sensor");
    }

    bool avail = BH1750.begin(BH1750_TO_GROUND); // init the sensor with address pin connetcted to ground
                                                 // result (bool) wil be be "false" if no sensor found
    if (!avail)
    {
        Serial.println("No BH1750 sensor found!");
    }

    envSensorTimer.every(ENV_SENSOR_INTERVAL * 1000, timerEvent);
    timerEvent(nullptr);
}

void EnvSensor::heartbeat()
{
    envSensorTimer.tick();
}

EnvData EnvSensor::update()
{
    sensors_event_t humidity, temp;

    shtc3.getEvent(&humidity, &temp);

    float fConversion = temp.temperature * 1.8 + 32;

    BH1750.start(); // starts a measurement
    float lux = BH1750.getLux();

    return {round(fConversion), round(humidity.relative_humidity), lux};
}

void EnvSensor::setCallback(EnvSensorCallback callback)
{
    envDataUpdateFunction = callback;
}