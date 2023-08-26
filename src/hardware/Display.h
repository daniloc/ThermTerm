#ifndef SRC_HARDWARE_DISPLAY
#define SRC_HARDWARE_DISPLAY

#include "utility/SingletonTemplate.h"

#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

class Display: public Singleton<Display>
{
    friend class Singleton<Display>;

public:

enum StatusLight {
    Red,
    Blue,
    White,
    Off
};

    Adafruit_ST7789 &configure();
    void clearScreen();

    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void dimScreen();
    void brightScreen();

    void setStatusLight(StatusLight);

private:
    void drawInitScreen();
    Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
};

#endif // SRC_HARDWARE_DISPLAY
