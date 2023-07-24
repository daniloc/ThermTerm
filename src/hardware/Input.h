#ifndef SRC_HARDWARE_INPUT
#define SRC_HARDWARE_INPUT

#include "utility/SingletonTemplate.h"

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

private:
    bool readRotaryButton();
    RotaryDirection readRotaryPosition();
    void handleRotaryButton();
    void handleRotaryUp();
    void handleRotaryDown();

    static InputHandlingCallback inputHandlingCallback;
};

#endif // SRC_HARDWARE_INPUT
