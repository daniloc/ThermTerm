#ifndef SRC_VIEWS_DIALVIEW
#define SRC_VIEWS_DIALVIEW

#include "BaseView.h"
#include "SystemController.h"

class DialView : public BaseView
{
public:
    DialView(Adafruit_ST7789 &tft,
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

#endif // SRC_VIEWS_DIALVIEW
