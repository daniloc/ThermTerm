#ifndef SRC_HARDWARE_INPUT
#define SRC_HARDWARE_INPUT

#include "utility/SingletonTemplate.h"
#include "_Constants.h"

#include "RotaryEncoder.h"

#include <AceButton.h>
using namespace ace_button;

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

    UserInput(InputSource source) : source(source)
    {
        if (source == RotaryDown || source == RotaryUp)
        {
            event = ValueChange;
        }
        else
        {
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
    void setCallback(InputHandlingCallback callback) { inputHandlingCallback = callback;};
    void processInput(UserInput input);
    void rotaryEncoderHandler(RotaryEncoder::Event);
    seesaw_NeoPixel *getNeoPixel() { return rotaryEncoder_.getNeopixel(); };
    static void handleButton(AceButton *button, uint8_t eventType, uint8_t buttonState);

private:
    bool readRotaryButton();
    void handleRotaryButton();
    void handleRotaryUp();
    void handleRotaryDown();
    RotaryEncoder rotaryEncoder_;

    static InputHandlingCallback inputHandlingCallback;
};

#endif // SRC_HARDWARE_INPUT
