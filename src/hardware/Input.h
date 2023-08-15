#ifndef SRC_HARDWARE_INPUT
#define SRC_HARDWARE_INPUT

#include "utility/SingletonTemplate.h"

#include "Adafruit_seesaw.h"
#include "seesaw_neopixel.h"

#define ENCODER_NEOPIXEL 6

enum RotaryDirection
{
    Unchanged,
    Up,
    Down
};

enum class InputEvent
{
    RotaryUp,
    RotaryDown,
    RotaryButton,
    Button0,
    Button1,
    Button2
};

typedef void (*InputHandlingCallback)(InputEvent event);

class Input : public Singleton<Input>
{
    friend class Singleton<Input>;

public:
    void configure();
    void heartbeat();
    void setCallback(InputHandlingCallback callback);
    void processInput(InputEvent input);
    seesaw_NeoPixel* getNeoPixel() { return &encoderPixel_; };

private:
    bool readRotaryButton();
    RotaryDirection readRotaryPosition();
    void handleRotaryButton();
    void handleRotaryUp();
    void handleRotaryDown();

    static InputHandlingCallback inputHandlingCallback;
    Adafruit_seesaw rotaryEncoder;
    seesaw_NeoPixel encoderPixel_ = seesaw_NeoPixel(1, ENCODER_NEOPIXEL, NEO_GRB + NEO_KHZ800);
};

#endif // SRC_HARDWARE_INPUT
