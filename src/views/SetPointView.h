#ifndef SRC_VIEWS_DIALVIEW_20COPY
#define SRC_VIEWS_DIALVIEW_20COPY

#include "BaseView.h"
#include "model/StateContainer.h"

class SetPointView : public BaseView
{
public:
    SetPointView(Adafruit_ST7789 &tft,
                 StateContainer &state,
                 BaseView &superview) : BaseView(tft),
                                        state_(state),
                                        superview_(superview)
    {
    }

    void draw() override;
    void handleInputEvent(InputEvent event) override;

private:
    StateContainer &state_;
    BaseView &superview_;
    void objectDidChange() override;
};

#endif