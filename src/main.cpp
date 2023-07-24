#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include "views/BaseView.h"
#include "hardware/Display.h"
#include "hardware/EnvSensor.h"
#include "hardware/Input.h"

#include "views/StatusView.h"
#include "StateContainer.h"

StateContainer state;
EnvSensor sensor = EnvSensor();
Input &input = Input::shared();

Adafruit_ST7789 &tft = Display::shared().configure();
StatusView statusView(tft, state);
BaseView &activeView = statusView;

void handleInput(InputEvent event) {
  Serial.print(F("forwarded input"));
  activeView.handleInputEvent(event);
}

void updateEnvironmentData(float temp, float humidity)
{

  if (temp != state.getTemperature() || humidity != state.getHumidity()) {

  state.setHumidity(humidity);
  state.setTemperature(temp);

  statusView.draw();

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
}

void loop()
{
  sensor.heartbeat();
  input.heartbeat();
}
