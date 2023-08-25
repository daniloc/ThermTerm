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
  SystemState stateData = controller_.getState();

  // populate text based on state values
  // (you'll need to convert the floats to strings)

  uint16_t tempMeterColor;

  if (stateData.power == OFF)
  {
    tempMeterColor = ST77XX_WHITE;
    Input::shared().getNeoPixel()->clear();
  }
  else if (stateData.hvacMode == HVAC_HOT)
  { // heat
    tempMeterColor = ST77XX_RED;
    
    Input::shared().getNeoPixel()->setPixelColor(0, 255, 0, 0);
  }
  else if (stateData.hvacMode == HVAC_COLD)
  { // cool
    tempMeterColor = ST77XX_BLUE;
    Input::shared().getNeoPixel()->setPixelColor(0, 0, 0, 255);
  }
  else
  { // other modes
    tempMeterColor = ST77XX_WHITE;
    Input::shared().getNeoPixel()->setPixelColor(0, 100, 100, 100);
  }

  if (controller_.shouldDimScreen()) {
    Input::shared().getNeoPixel()->clear();
  } else {
    Input::shared().getNeoPixel()->setBrightness(40);
  }

  Input::shared().getNeoPixel()->show();

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
    viewHierarchy.push(&setPointView_);
    controller_.incrementSetPoint();
    break;
  case InputEvent::RotaryDown:
    viewHierarchy.push(&setPointView_);
    controller_.decrementSetPoint();
    break;
  case InputEvent::Button0:
    controller_.setHVACMode(HVAC_COLD);
    break;
  case InputEvent::Button1:
    viewHierarchy.push(&dialView_);
    draw();
    break;
  case InputEvent::Button2:
    controller_.setHVACMode(HVAC_HOT);
    break;
  case InputEvent::RotaryButton:
    controller_.togglePower();
    break;
  default:
    break;
  }
}

void StatusView::objectDidChange()
{
  draw();
}

void StatusView::heartbeat()
{
  if (millis() - lastInputEventTime >= modalViewDuration)
  {
    // Dismiss the modal view if needed
    if (!viewHierarchy.empty())
    {
      viewHierarchy.pop();
      lastInputEventTime = millis(); // Reset the timer after dismissing the view
      Display::shared().clearScreen();
      draw();
    }
  }
}