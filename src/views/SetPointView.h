#ifndef SRC_VIEWS_DIALVIEW_20COPY
#define SRC_VIEWS_DIALVIEW_20COPY

#include "BaseView.h"
#include "SystemController.h"

class SetPointView : public BaseView
{
public:
    SetPointView(Adafruit_ST7789 &tft,
                 SystemController &controller,
                 BaseView &superview) : BaseView(tft),
                                        controller_(controller),
                                        superview_(superview)
    {
    }

    void draw() override;
    void handleInputEvent(InputEvent event) override;

private:
    SystemController &controller_;
    BaseView &superview_;
    void objectDidChange() override;
};

#endif