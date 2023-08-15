#include "StatusView.h"
#include "hardware/Display.h"

#include <Adafruit_GFX.h> // Core graphics library

#define humidityMeterColor 0x0C34
#define modalViewDuration 3000

void StatusView::draw()
{
  if (viewHierarchy.size() > 0)
  {
    viewHierarchy.front()->draw();
    return;
  }

  // retrieve values from state
  StateData stateData = state.getState();

  // populate text based on state values
  // (you'll need to convert the floats to strings)

  uint16_t tempMeterColor;

  if (stateData.power == OFF)
  {
    tempMeterColor = ST77XX_WHITE;
  }
  else if (stateData.hvacMode == HVAC_HOT)
  { // heat
    tempMeterColor = ST77XX_RED;
  }
  else if (stateData.hvacMode == HVAC_COLD)
  { // cool
    tempMeterColor = ST77XX_BLUE;
  }
  else
  { // other modes
    tempMeterColor = ST77XX_WHITE;
  }

  tft.setTextColor(tempMeterColor, ST77XX_BLACK);

  tft.setTextSize(7);
  BaseView::drawTextRightAligned(0, String(int(stateData.temperature)) + "F");

  tft.setTextColor(humidityMeterColor, ST77XX_BLACK);
  tft.setTextSize(4);
  BaseView::drawTextRightAligned(60, String(int(stateData.humidity)) + "%RH");

  tft.setTextSize(2);

  if (stateData.power == ON)
  {
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  }
  else
  {
    tft.setTextColor(ST77XX_BLACK, ST77XX_BLACK);
  }

  BaseView::drawTextRightAligned(100, "Set: " + String(stateData.setPoint));

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

  tft.setTextSize(2);

  tft.setCursor(0, 0);
  tft.print("COOL");

  tft.setCursor(0, (height / 2) - 10);
  tft.print("SET FAN");

  tft.setCursor(0, height - 20);
  tft.print("HEAT");
}

void StatusView::handleInputEvent(InputEvent event)
{

  lastInputEventTime = millis();

  if (viewHierarchy.size() > 0)
  {
    Serial.print("redirecting input");
    viewHierarchy.front()->handleInputEvent(event);

    return;
  }

  switch (event)
  {
  case InputEvent::RotaryUp:
    state.incrementSetPoint();
    break;
  case InputEvent::RotaryDown:
    state.decrementSetPoint();
    break;
  case InputEvent::Button0:
    state.setHVACMode(HVAC_COLD);
    break;
  case InputEvent::Button1:
    viewHierarchy.push(&dialView_);
    draw();
    break;
  case InputEvent::Button2:
    state.setHVACMode(HVAC_HOT);
    break;
  case InputEvent::RotaryButton:
    state.turnOff();
    break;
  default:
    break;
  }

}

void StatusView::objectDidChange()
{
  draw();
}

void StatusView::heartbeat() {
  if (millis() - lastInputEventTime >= modalViewDuration)
  {
    // Dismiss the dial view
    if (!viewHierarchy.empty() && viewHierarchy.front() == &dialView_)
    {
      viewHierarchy.pop();
      lastInputEventTime = millis(); // Reset the timer after dismissing the view
      Display::shared().clearScreen();
      draw();
    }
  }
}