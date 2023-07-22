#ifndef SRC_VIEWS_STATUSVIEW
#define SRC_VIEWS_STATUSVIEW

#include "BaseView.h"

#include <Adafruit_ST7789.h>

class StatusView : public BaseView
{
public:
    StatusView(Adafruit_ST7789 &tft) : BaseView(tft) {}

    void draw() override;
};

#endif // SRC_VIEWS_STATUSVIEW
