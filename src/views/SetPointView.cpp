#include "SetPointView.h"
#include "hardware/Display.h"
#include "hardware/Input.h"

void SetPointView::draw()
{

    Display::shared().clearScreen();

    tft.setTextSize(4);
    tft.setCursor(0, 0);

    switch (controller_.getState().hvacMode)
    {
    case HvacMode::HVAC_HOT:
        tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
        tft.print("Heat to:");

        break;
    case HvacMode::HVAC_COLD:
        tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
        tft.print("Cool to:");
        break;
    default:
        break;
    }

    tft.setCursor(0, 60);                                // set cursor position, adjust as necessary
    tft.setTextSize(7);                                  // set text size, adjust as necessary
    tft.print(String(int(controller_.getState().setPoint))); // print the string
}

void SetPointView::handleInputEvent(InputEvent event)
{
    switch (event)
    {
    case InputEvent::RotaryUp:
        controller_.incrementSetPoint();
         break;

    case InputEvent::RotaryDown:
         controller_.decrementSetPoint();
         break;

    case InputEvent::RotaryButton:
        break;

    default:
        break;
    }
    draw();
}

void SetPointView::objectDidChange()
{
    draw();
}