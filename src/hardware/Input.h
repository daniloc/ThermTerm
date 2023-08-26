#ifndef SRC_HARDWARE_INPUT
#define SRC_HARDWARE_INPUT

#include "utility/SingletonTemplate.h"
#include "_Constants.h"

#include "Adafruit_seesaw.h"
#include "seesaw_neopixel.h"

enum RotaryDirection
{
    Unchanged,
    Up,
    Down
};

enum InputSource
{
    RotaryUp,
    RotaryDown,
    RotaryButton,
    Button0,
    Button1,
    Button2
};

enum InputEvent
{
    Click,
    DoubleClick,
    LongPress,
    ValueChange
};

struct UserInput
{
    InputSource source;
    InputEvent event = Click;

    UserInput(InputSource source) : source(source) {
        if (source == RotaryDown || source == RotaryUp) {
            event = ValueChange;
        } else {
            event = Click;
        }
    }
    UserInput(InputSource source, InputEvent event) : source(source), event(event) {}
};

typedef void (*InputHandlingCallback)(UserInput event);

class Input : public Singleton<Input>
{
    friend class Singleton<Input>;

public:
    void configure();
    void heartbeat();
    void setCallback(InputHandlingCallback callback);
    void processInput(UserInput input);
    seesaw_NeoPixel *getNeoPixel() { return &encoderPixel_; };

private:
    bool readRotaryButton();
    RotaryDirection readRotaryPosition();
    void handleRotaryButton();
    void handleRotaryUp();
    void handleRotaryDown();

    static InputHandlingCallback inputHandlingCallback;
    Adafruit_seesaw rotaryEncoder;
    seesaw_NeoPixel encoderPixel_ = seesaw_NeoPixel(1, ENCODER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
};

#endif // SRC_HARDWARE_INPUT
