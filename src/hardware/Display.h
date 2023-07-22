#ifndef SRC_HARDWARE_DISPLAY
#define SRC_HARDWARE_DISPLAY

#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

class Display
{
public:
    static Display &shared()
    {
        static Display instance;
        return instance;
    }

    Adafruit_ST7789 &configure();
    void clearScreen();

    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    void drawInitScreen();
    Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

    Display(); // Private constructor

    // Prevent copying and assignment
    Display(const Display &) = delete;
    void operator=(const Display &) = delete;
};

#endif // SRC_HARDWARE_DISPLAY
