#include "Settings.h"

#include <SimpleCLI.h>
#include <Preferences.h>

SimpleCLI cli;

Preferences preferences;

const char *ssidKey = "ssid";
const char *passwordKey = "pass";
const char *deviceKey = "device";

void newline()
{
    Serial.print("\n");
}

void prompt()
{
    Serial.print("\n>");
}

void setValueForKey(String value, const char *key)
{
    preferences.putString(key, value);
}

void handleSSID(cmd *c)
{
    Command command(c);

    String newSSID = command.getArgument(0).getValue();

    if (newSSID.length() > 0)
    {
        setValueForKey(newSSID, ssidKey);

        Serial.print("New SSID set: ");
        Serial.print(newSSID);
    }
    else
    {
        Serial.print("That SSID wasn't long enough.");
    }
}

void handlePassword(cmd *c)
{
    Command command(c);
    setValueForKey(command.getArgument(0).getValue(), passwordKey);

    Serial.print("New password set.");
}

void handleClearPassword(cmd *c)
{
    preferences.remove(passwordKey);

    Serial.print("Password cleared.");
}

void handleDeviceName(cmd *c)
{
    Command command(c);
    setValueForKey(command.getArgument(0).getValue(), deviceKey);

    String newDeviceName = command.getArgument(0).getValue();

    if (newDeviceName.length() > 3)
    {
        setValueForKey(newDeviceName, deviceKey);

        Serial.print("New name set: ");
        Serial.print(newDeviceName);
    }
    else
    {
        Serial.print("That name wasn't long enough.");
    }
}

void handleReset(cmd *c)
{
    Serial.print("Storing settings...");
    preferences.end();
    delay(1000);
    newline();
    Serial.print("Restarting...");
    ESP.restart();
}

Command setSSID = cli.addSingleArgumentCommand("ssid", handleSSID);
Command setPassword = cli.addSingleArgumentCommand("pass", handlePassword);
Command clearPassword = cli.addSingleArgumentCommand("clearpass", handleClearPassword);
Command setDeviceName = cli.addSingleArgumentCommand("name", handleDeviceName);
Command reset = cli.addSingleArgumentCommand("save", handleReset);

bool Settings::needsConfiguration()
{
    String ssid = preferences.getString(ssidKey);

    if (ssid.length() == 0)
    {
        newline;
        return true;
    }

    String deviceName = preferences.getString(deviceKey);

    if (deviceName.length() == 0)
    {
        newline;
        return true;
    }

    return false;
}

void Settings::printWelcomePrompt()
{
    newline();
    newline();
    Serial.print("======= Welcome to ThermTerm =======\n");

    if (preferences.begin("enviropad", false))
    {
        Serial.print("Preferences ready for input.\n");
        Serial.print("Awaiting configuration. Enter commands and values to continue.\n");
        Serial.print("Use 'ssid [network name]' to choose WiFi network.\n");
        Serial.print("Use 'pass [password]' to set WiFi password if needed.\n");
        Serial.print("Use 'clearpass' to clear WiFi password.\n");
        Serial.print("Use 'name [device name]' to choose a unique name for this device.\n");
        Serial.print("'save' will store your changes and reboot the device.\n");
        prompt();
    }
    else
    {
        Serial.print("There was an error opening the preferences store.");
    }
}

void Settings::heartbeat()
{

    if (Serial.available())
    {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');
        Serial.print(input);

        // Parse the user input into the CLI
        cli.parse(input);

        if (cli.errored())
        {
            CommandError error = cli.getError();

            Serial.print("Could not parse: ");
            Serial.println(error.toString());
        }

        prompt();
    }
}