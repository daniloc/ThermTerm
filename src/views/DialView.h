#ifndef SRC_VIEWS_DIALVIEW
#define SRC_VIEWS_DIALVIEW

#include "BaseView.h"
#include "model/StateContainer.h"

class DialView : public BaseView
{
public:
    DialView(Adafruit_ST7789 &tft,
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

#endif // SRC_VIEWS_DIALVIEW
