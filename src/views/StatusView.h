#ifndef SRC_VIEWS_STATUSVIEW
#define SRC_VIEWS_STATUSVIEW

#include "BaseView.h"
#include "StateContainer.h"

#include <Adafruit_ST7789.h>

class StatusView : public BaseView
{
public:
    StatusView(Adafruit_ST7789 &tft, StateContainer &state) : BaseView(tft), state(state) {} // add state argument

    void draw() override;
    void handleInputEvent(InputEvent event) override;

private:
    StateContainer &state;
};

#endif // SRC_VIEWS_STATUSVIEW
