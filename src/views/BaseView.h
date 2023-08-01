#ifndef SRC_VIEWS_BASEVIEW
#define SRC_VIEWS_BASEVIEW

#include "hardware/Display.h"
#include "hardware/Input.h"

#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include "utility/Observation.h"
#include <queue>

class BaseView : public Observer
{
public:
    BaseView(Adafruit_ST7789 &tft)
        : tft(tft), width(Display::shared().getWidth()), height(Display::shared().getHeight()) {}

    virtual void draw() = 0; // declare a pure virtual function

    virtual void handleInputEvent(InputEvent event) = 0;

    std::queue<BaseView*> viewHierarchy;

protected:
    void drawTextRightAligned(int y, const String &text)
    {
        int16_t x1, y1;
        uint16_t w, h;
        tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h); // calc width of text
        tft.setCursor(width - w, y);                     // set cursor position
        tft.println(text);
    }

    Adafruit_ST7789 &tft;
    uint16_t width;
    uint16_t height;
};
#endif // SRC_VIEWS_BASEVIEW
