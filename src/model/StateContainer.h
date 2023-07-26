#ifndef SRC_MODEL_STATECONTAINER
#define SRC_MODEL_STATECONTAINER

#include "utility/SingletonTemplate.h"
#include "model/StateData.h"

#include <ArduinoHA.h>
#include <WiFi.h>

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
    StateData getState();

    // setters
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setSetPoint(float setPoint);
    void setHVACMode(HvacMode hvacMode);
    void setFanSpeed(HvacFanMode fanSpeed);

    void incrementSetPoint();
    void decrementSetPoint();

    void heartbeat();
    void configure();

private:
    StateData stateData_;
    void updateMitsubishiInterface();
    String macAddress_;
    HADevice haDevice_;
    HAMqtt mqtt_;
    HASensorNumber humiditySensor_;
    HASensorNumber temperatureSensor_;
};

#endif // SRC_MODEL_STATECONTAINER
