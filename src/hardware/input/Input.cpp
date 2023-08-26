#include "Input.h"

#include <queue>

std::queue<UserInput> eventQueue;

InputHandlingCallback Input::inputHandlingCallback = nullptr;

ButtonConfig buttonConfig;
AceButton button0(&buttonConfig, BUTTON0_PIN, HIGH);
AceButton button1(&buttonConfig, BUTTON1_PIN, LOW);
AceButton button2(&buttonConfig, BUTTON2_PIN, LOW);

void Input::configure()
{
    pinMode(BUTTON0_PIN, INPUT_PULLUP);

    buttonConfig.setEventHandler(handleButton);
    rotaryEncoder_.configure();
}

void Input::processInput(UserInput input)
{
    if (inputHandlingCallback != nullptr)
    {
        inputHandlingCallback(input);
    }
}

void Input::rotaryEncoderHandler(RotaryEncoder::Event event)
{
    switch (event)
    {
    case RotaryEncoder::Event::Up:
        eventQueue.push(UserInput(RotaryUp));
        break;
    case RotaryEncoder::Event::Down:
        eventQueue.push(UserInput(RotaryDown));
        break;
    case RotaryEncoder::Event::Clicked:
        eventQueue.push(UserInput(RotaryButton));
    }
}

void Input::handleButton(AceButton *button, uint8_t eventType, uint8_t buttonState)
{

    if (eventType == AceButton::kEventPressed)
    {
        return;
    }

    InputSource source = Button0;

    if (button == &button1)
    {
        source = Button1;
    }
    else if (button == &button2)
    {
        source = Button2;
    }

    Input::shared().processInput(UserInput(source));
}

void Input::heartbeat()
{

    while (!eventQueue.empty())
    {
        inputHandlingCallback(eventQueue.front());
        eventQueue.pop();
    }

    rotaryEncoder_.heartbeat();
    button0.check();
    button1.check();
    button2.check();
}
