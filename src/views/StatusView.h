#ifndef SRC_VIEWS_STATUSVIEW
#define SRC_VIEWS_STATUSVIEW

#include "BaseView.h"
#include "model/StateContainer.h"
#include "views/DialView.h"
#include "SetPointView.h"

#include <Adafruit_ST7789.h>

class StatusView : public BaseView
{
public:
    StatusView(Adafruit_ST7789 &tft, StateContainer &state)
        : BaseView(tft),
          state(state),
          dialView_(tft, state, *this),
          setPointView_(tft, state, *this)
    {
        state.registerObserver(this);
    }

    void draw() override;
    void handleInputEvent(InputEvent event) override;
    void heartbeat();

private:
    StateContainer &state;
    DialView dialView_;
    SetPointView setPointView_;
    void objectDidChange() override;
    unsigned long lastInputEventTime = 0;
};

#endif // SRC_VIEWS_STATUSVIEW
