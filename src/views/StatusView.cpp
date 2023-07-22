#include "StatusView.h"
#include "hardware/Display.h"

#include <Adafruit_GFX.h> // Core graphics library

void StatusView::draw()
{
  Display::shared().clearScreen();

  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(7);
  BaseView::drawTextRightAligned(0, "60F");

  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  BaseView::drawTextRightAligned(60, "52%RH");

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  BaseView::drawTextRightAligned(100, "Set: 62");

  tft.setTextSize(2);

  tft.setCursor(0, 0);
  tft.print("COOL");

  tft.setCursor(0, (height / 2) - 10);
  tft.print("OFF");

  tft.setCursor(0, height - 20);
  tft.print("HEAT");
}