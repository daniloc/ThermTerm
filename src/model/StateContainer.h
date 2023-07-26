#ifndef SRC_MODEL_STATECONTAINER
#define SRC_MODEL_STATECONTAINER

#include "utility/SingletonTemplate.h"

#include <ArduinoHA.h>
#include <WiFi.h>

enum class HVACMode
{
    OFF,
    FAN,
    COOL,
    HOT
};

class StateContainer
{
public:
    // constructors
    StateContainer();

    static String generateMacAddress()
    {
        String macAddress = WiFi.macAddress();
        macAddress.replace(":", "");
        return macAddress;
    }

    // getters
    float getTemperature() const;
    float getHumidity() const;
    float getSetPoint() const;
    HVACMode getHVACMode() const;
    int getFanSpeed() const;

    // setters
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setSetPoint(float setPoint);
    void setHVACMode(HVACMode hvacMode);
    void setFanSpeed(int fanSpeed);

    void incrementSetPoint();
    void decrementSetPoint();

    void heartbeat();
    void configure();

private:
    float temperature_;
    float humidity_;
    float setPoint_;
    HVACMode hvacMode_;
    int fanSpeed_;
    void updateMitsubishiInterface();
    String macAddress_;
    HADevice haDevice_;
    HAMqtt mqtt_;
    HASensorNumber humiditySensor_;
    HASensorNumber temperatureSensor_;
};

#endif // SRC_MODEL_STATECONTAINER
