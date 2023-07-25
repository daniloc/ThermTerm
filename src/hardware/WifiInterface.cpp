#include "WifiInterface.h"
#include "wifiCredentials.h"

WifiInterface::WifiInterface()
{
}

void WifiInterface::configure()
{
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
}

WiFiClient &WifiInterface::getWifiClient()
{
    return client_;
}