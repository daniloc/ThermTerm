#ifndef SRC_VIEWS_STATUSVIEW
#define SRC_VIEWS_STATUSVIEW

#include "BaseView.h"
#include "SystemController.h"
#include "views/DialView.h"
#include "SetPointView.h"

#include <Adafruit_ST7789.h>

class StatusView : public BaseView
{
public:
    StatusView(Adafruit_ST7789 &tft, SystemController &controller)
        : BaseView(tft),
          controller_(controller),
          dialView_(tft, controller, *this),
          setPointView_(tft, controller, *this)
    {
        controller.registerObserver(this);
    }

    void draw() override;
    void handleInputEvent(UserInput) override;
    void heartbeat();

private:
    SystemController &controller_;
    DialView dialView_;
    SetPointView setPointView_;
    void objectDidChange() override;
    unsigned long lastInputEventTime = 0;
};

#endif // SRC_VIEWS_STATUSVIEW
