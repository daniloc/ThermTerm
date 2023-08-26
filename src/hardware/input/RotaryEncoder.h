#ifndef SRC_HARDWARE_INPUT_ROTARYENCODER
#define SRC_HARDWARE_INPUT_ROTARYENCODER

#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include "_Constants.h"

class RotaryEncoder
{
    enum Direction
    {
        Unchanged,
        Left,
        Right
    };


public:
    enum Event
    {
        Up,
        Down,
        Clicked
    };

    void heartbeat();
    void configure();
    seesaw_NeoPixel *getNeopixel() { return &neoPixel_;};

private:
    Adafruit_seesaw seesawInterface_;
    seesaw_NeoPixel neoPixel_ = seesaw_NeoPixel(1, ENCODER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
    Direction readRotaryPosition();
    bool readButton();
    bool lastButtonState = false;
    bool buttonState = true;
    unsigned long lastButtonDebounceTime = 0;
    unsigned long debounceInterval = 50;
    int32_t encoderPosition = 0;
};

#endif // SRC_HARDWARE_INPUT_ROTARYENCODER
