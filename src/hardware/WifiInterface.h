#ifndef SRC_HARDWARE_WIFIINTERFACE
#define SRC_HARDWARE_WIFIINTERFACE

#include "utility/SingletonTemplate.h"

#include <WiFi.h>

class WifiInterface : public Singleton<WifiInterface>
{
    friend class Singleton<WifiInterface>;

public:
    void configure();
    WiFiClient &getWifiClient();

private:
    WiFiClient client_;
    WifiInterface();
};

#endif // SRC_HARDWARE_WIFIINTERFACE
