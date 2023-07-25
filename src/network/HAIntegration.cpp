#include "HAIntegration.h"
#include "wifiCredentials.h"
#include <ArduinoHA.h>
#include <WiFi.h>

// Adapted via:
//   https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/nano33iot/nano33iot.ino

#define LED_PIN LED_BUILTIN

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

void HAIntegration::configure()
{

    // Prepare LED:

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Set device ID as MAC address

    byte mac[32];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    // Device metadata:

    device.setName("Enviropad");
    device.setSoftwareVersion("0.1");


    Serial.print("Connecting to MQTT\n");

    if (mqtt.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true)
    {
        Serial.print("Connected to MQTT broker");
    }
    else
    {
        Serial.print("Could not connect to MQTT broker");
    }
}

void HAIntegration::heartbeat()
{
    mqtt.loop();
}