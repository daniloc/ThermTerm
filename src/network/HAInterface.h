#ifndef SRC_NETWORK_HAINTERFACE
#define SRC_NETWORK_HAINTERFACE

#include <ArduinoHA.h>
#include <WiFi.h>

class HAInterface
{

public:
    HAInterface();
    void configure(bool);
    void heartbeat();

    static String generateMacAddress()
    {
        String macAddress = WiFi.macAddress();
        macAddress.replace(":", "");
        return macAddress;
    }

    // Getter methods
    HASensorNumber &getHumiditySensor() { return humiditySensor_; }
    HASensorNumber &getTemperatureSensor() { return temperatureSensor_; }
    HASensorNumber &getLightSensor() { return lightSensor_; }
    HASwitch &getAlertTrigger() { return alertTrigger_; }
    HAHVAC &getHVACDevice() { return hvacDevice_; }

private:
    String macAddress_;
    HADevice haDevice_;
    HAMqtt mqtt_;
    HASensorNumber humiditySensor_;
    HASensorNumber temperatureSensor_;
    HASensorNumber lightSensor_;
    HASwitch alertTrigger_;
    HAHVAC hvacDevice_;
};

#endif // SRC_NETWORK_HAINTERFACE
