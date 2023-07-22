#include <Arduino.h>
#include <Adafruit_ST7789.h>

#include "hardware/Display.h"
#include "views/StatusView.h"


void setup(void)
{
  Serial.begin(9600);
  Serial.print(F("Hello! Feather TFT Test"));

  Adafruit_ST7789 &tft = Display::shared().configure();
  StatusView statusView(tft);
  statusView.draw();
}

void loop()
{

}
