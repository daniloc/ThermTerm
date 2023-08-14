
#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include "views/BaseView.h"
#include "hardware/Display.h"
#include "hardware/EnvSensor.h"
#include "hardware/Input.h"

#include "views/StatusView.h"
#include "model/StateContainer.h"

#include "hardware/WifiInterface.h"
#include <WiFi.h>



StateContainer state;
EnvSensor sensor = EnvSensor();
Input &input = Input::shared();

Adafruit_ST7789 &tft = Display::shared().configure();
StatusView statusView(tft, state);
BaseView &activeView = statusView;

void handleInput(InputEvent event)
{
  Serial.print(F("forwarded input"));
  activeView.handleInputEvent(event);
}

void updateEnvironmentData(float temp, float humidity, float lux)
{

  if (temp != state.getState().temperature || humidity != state.getState().humidity)
  {

    state.setHumidity(humidity);
    state.setTemperature(temp);

    statusView.draw();
  }

  if (lux < 20) {
    Display::shared().dimScreen();
  } else {
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

  WifiInterface::shared().configure();
  state.configure();
}

void loop()
{
  sensor.heartbeat();
  input.heartbeat();
  state.heartbeat();
  statusView.heartbeat();
}
