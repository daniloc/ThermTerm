#include "EnvSensor.h"
#include "Adafruit_SHTC3.h"
#include <arduino-timer.h>

Adafruit_SHTC3 shtc3;

auto envSensorTimer = timer_create_default();

#define ENV_SENSOR_INTERVAL 5 //Seconds

EnvSensorCallback EnvSensor::envDataUpdateFunction = nullptr;

bool EnvSensor::timerEvent(void *argument) {

    // Update sensor data
    EnvData envData = update();

    // If a callback function has been set, call it with the updated data
    if (envDataUpdateFunction != nullptr)
    {
        envDataUpdateFunction(envData.temp, envData.humidity);
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

    envSensorTimer.every(ENV_SENSOR_INTERVAL * 1000, timerEvent);
    timerEvent(nullptr);
}

void EnvSensor::heartbeat() {
    envSensorTimer.tick();
}

EnvData EnvSensor::update()
{
    sensors_event_t humidity, temp;

    shtc3.getEvent(&humidity, &temp);

    float fConversion = temp.temperature * 1.8 + 32;

    return {round(fConversion), round(humidity.relative_humidity)};
}

void EnvSensor::setCallback(EnvSensorCallback callback)
{
    envDataUpdateFunction = callback;
}