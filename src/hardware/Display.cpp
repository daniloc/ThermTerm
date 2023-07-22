#include "Display.h"

#include <Adafruit_GFX.h> // Core graphics library

uint16_t screenFillColor = ST77XX_BLACK;

#define SCREEN_WIDTH 240  // display width, in pixels
#define SCREEN_HEIGHT 135 // display height, in pixels

void Display::drawInitScreen()
{
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.print("Enviropad Initialized");
}

void Display::clearScreen()
{
  tft.fillScreen(screenFillColor);
}

Adafruit_ST7789 &Display::configure()
{
  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

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
