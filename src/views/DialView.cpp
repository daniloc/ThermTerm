#include "DialView.h"
#include "hardware/Display.h"
#include "hardware/Input.h"

void DialView::draw() {

    Display::shared().clearScreen();

    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

    tft.setTextSize(4);

    tft.setCursor(0, 0);
    tft.print("Fan Mode");

    String output;
    switch (state_.getFanSpeed())
    {
    case FAN_SPEED_1:
        output = "*";
        break;
    case FAN_SPEED_2:
        output = "**";
        break;
    case FAN_SPEED_3:
        output = "***";
        break;
    case FAN_SPEED_4:
        output = "****";
        break;
    case FAN_SPEED_5:
        output = "*****";
        break;
    case FAN_SPEED_AUTO:
        output = "Auto";
        break;
    case FAN_SPEED_SILENT:
        output = "Silent";
        break;
    }

    tft.setCursor(0, 60);            // set cursor position, adjust as necessary
    tft.setTextColor(ST77XX_WHITE); // set text color, adjust as necessary
    tft.setTextSize(3);             // set text size, adjust as necessary
    tft.print(output);            // print the string
}

void DialView::handleInputEvent(InputEvent event)
{
    switch (event)
    {
    case InputEvent::RotaryUp:
        // Handle increase in fan mode
        if (state_.getFanSpeed() >= FAN_SPEED_SILENT)
        {
            // If fan mode is at the maximum, loop around to the minimum
            state_.setFanSpeed(FAN_SPEED_1);
        }
        else
        {
            // Otherwise, just increment the fan mode
            state_.setFanSpeed((HvacFanMode)(state_.getFanSpeed() + 1));
        }
        break;

    case InputEvent::RotaryDown:
        // Handle decrease in fan mode
        if (state_.getFanSpeed() <= FAN_SPEED_1)
        {
            // If fan mode is at the minimum, loop around to the maximum
            state_.setFanSpeed(FAN_SPEED_SILENT);
        }
        else
        {
            // Otherwise, just decrement the fan mode
            state_.setFanSpeed((HvacFanMode)(state_.getFanSpeed() - 1));
        }
        break;

    case InputEvent::RotaryButton:
        break;

    default:
        break;
    }
    draw();
}

void DialView::objectDidChange()
{
    draw();
}