
#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include "views/BaseView.h"
#include "hardware/Display.h"
#include "hardware/EnvSensor.h"
#include "hardware/Audio.h"
#include "hardware/input/Input.h"
#include "hardware/infrared/MitsubishiInterface.h"

#include "views/StatusView.h"
#include "SystemController.h"

#include "hardware/WifiInterface.h"
#include <WiFi.h>

MitsubishiInterface mitsubishiInterface; // Create alternative IR schemes by sublassing IRInterface
SystemController controller(mitsubishiInterface);

EnvSensor sensor = EnvSensor();

Input &input = Input::shared();
Adafruit_ST7789 &tft = Display::shared().configure();
StatusView statusView(tft, controller);
BaseView &activeView = statusView;

void handleInput(UserInput input)
{
  Serial.print(F("forwarded input"));
  activeView.handleInputEvent(input);
}

void updateEnvironmentData(float temp, float humidity, float lux)
{

  if (temp != controller.getState().temperature ||
      humidity != controller.getState().humidity ||
      lux != controller.getState().lux)
  {
    controller.setHumidity(humidity);
    controller.setTemperature(temp);
    controller.setLux(lux);
    statusView.draw();
  }

  if (controller.shouldDimScreen())
  {
    Display::shared().dimScreen();
  }
  else
  {
    Display::shared().brightScreen();
  }
}

void setup(void)
{
  Serial.begin(9600);
  Serial.print(F("Hello! Feather TFT Test"));

  sensor.setCallback(updateEnvironmentData);
  input.setCallback(handleInput);

  sensor.configure();
  input.configure();
  statusView.draw();

  Audio::configure();

  WifiInterface::shared().configure();
  controller.configure();
}

void loop()
{
  sensor.heartbeat();
  input.heartbeat();
  controller.heartbeat();
  statusView.heartbeat();
}
