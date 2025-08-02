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

  // initialize tempMeterColor with a default value
  uint16_t tempMeterColor;
  if (stateData.power == OFF)
  {
    tempMeterColor = ST77XX_WHITE;
  }
  else
  {
    switch (stateData.hvacMode)
    {
    case HVAC_HOT: // heat
      tempMeterColor = ST77XX_RED;
      break;

    case HVAC_COLD: // cool
      tempMeterColor = ST77XX_BLUE;
      break;

    default: // other modes
      tempMeterColor = ST77XX_WHITE;
      break;
    }
  }

  // Only set StatusLight if shouldDimScreen() returns false
  if (!controller_.shouldDimScreen())
  {
    if (stateData.power == OFF)
    {
      Display::shared().setStatusLight(Display::StatusLight::Off);
    }
    else
    {
      switch (stateData.hvacMode)
      {
      case HVAC_HOT: // heat
        Display::shared().setStatusLight(Display::StatusLight::Red);
        break;

      case HVAC_COLD: // cool
        Display::shared().setStatusLight(Display::StatusLight::Blue);
        break;

      default: // other modes
        Display::shared().setStatusLight(Display::StatusLight::White);
        break;
      }
    }
  }
  else
  {
    Display::shared().setStatusLight(Display::StatusLight::Off);
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

  BaseView::drawTextRightAligned(100, "Set: " + String(int(stateData.getSetPointUIValue())) + "  ");

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

  tft.setTextSize(2);

  tft.setCursor(0, 0);
  tft.print("COOL");

  tft.setCursor(0, (height / 2) - 10);
  tft.print("SET FAN");

  tft.setCursor(0, height - 20);
  tft.print("HEAT");
}

void StatusView::handleInputEvent(UserInput input)
{

  lastInputEventTime = millis();

  if (viewHierarchy.size() > 0)
  {
    Serial.print("redirecting input");
    viewHierarchy.front()->handleInputEvent(input);

    return;
  }

  switch (input.source)
  {
  case RotaryUp:
    if (controller_.getState().power == OFF)
    {
      return;
    };

    viewHierarchy.push(&setPointView_);
    controller_.incrementSetPoint();
    break;
  case RotaryDown:

    if (controller_.getState().power == OFF)
    {
      return;
    };

    viewHierarchy.push(&setPointView_);
    controller_.decrementSetPoint();
    break;
  case Button0:
    viewHierarchy.push(&setPointView_);
    controller_.setHVACMode(HVAC_COLD);
    break;
  case Button1:
    viewHierarchy.push(&dialView_);
    draw();
    break;
  case Button2:
    viewHierarchy.push(&setPointView_);
    controller_.setHVACMode(HVAC_HOT);
    break;
  case RotaryButton:
    if (controller_.getState().power == OFF)
    {
      viewHierarchy.push(&setPointView_);
    }
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
  if (millis() - lastInputEventTime >= modalViewDuration || shouldDismissModal == true)
  {
    // Dismiss the modal view if needed
    if (!viewHierarchy.empty())
    {
      viewHierarchy.pop();
      lastInputEventTime = millis(); // Reset the timer after dismissing the view
      Display::shared().clearScreen();
      draw();
    }

    shouldDismissModal = false;
  }
}