#include "Display.h"
#include "_Constants.h"

#include <Adafruit_GFX.h> // Core graphics library

uint16_t screenFillColor = ST77XX_BLACK;

void Display::drawInitScreen()
{
}

void Display::clearScreen()
{
  tft.fillScreen(screenFillColor);
}

void Display::dimScreen() {
  analogWrite(TFT_BACKLITE, 2);
}

void Display::brightScreen() {
  analogWrite(TFT_BACKLITE, 64);
}
 
Adafruit_ST7789 &Display::configure()
{
  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  brightScreen();
  
  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH); // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  Serial.println(F("Display initialized"));

  drawInitScreen();

  return tft;
}

uint16_t Display::getWidth() const { return SCREEN_WIDTH; }
uint16_t Display::getHeight() const { return SCREEN_HEIGHT; }
