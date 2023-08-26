#include "Input.h"

#include <queue>
#include <AceButton.h>
using namespace ace_button;

std::queue<UserInput> eventQueue;

InputHandlingCallback Input::inputHandlingCallback = nullptr;

volatile unsigned long lastInterruptTime0 = 0;
volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;
unsigned long debounceTime = 500; // Debounce time in milliseconds

void handleButton0Press()
{
    if ((millis() - lastInterruptTime0) > debounceTime)
    {
        Serial.println(F("Button 0 pressed"));
        eventQueue.push(UserInput(Button0));
    }
    lastInterruptTime0 = millis();
}

void handleButton1Press()
{
    if ((millis() - lastInterruptTime1) > debounceTime)
    {
        Serial.println(F("Button 1 pressed"));
        eventQueue.push(UserInput(Button1));
    }
    lastInterruptTime1 = millis();
}

void handleButton2Press()
{
    if ((millis() - lastInterruptTime2) > debounceTime)
    {
        Serial.println(F("Button 2 pressed"));
        eventQueue.push(UserInput(Button2));
    };

    lastInterruptTime2 = millis();
}

// *** Input Handlers

void Input::processInput(UserInput input)
{
    if (inputHandlingCallback != nullptr)
    {
        inputHandlingCallback(input);
    }
}

void Input::handleRotaryButton()
{
    Serial.print(F("Rotary button"));
    processInput(UserInput(RotaryButton));
}

void Input::handleRotaryUp()
{
    Serial.print(F("Rotary up"));
    processInput(UserInput(RotaryUp));
}

void Input::handleRotaryDown()
{
    Serial.print(F("Rotary down"));
    processInput(UserInput(RotaryDown));
}

void Input::rotaryEncoderHandler(RotaryEncoder::Event event) {
    switch(event) {
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

// *** Lifecycle

void Input::heartbeat()
{

    while (!eventQueue.empty())
    {
        inputHandlingCallback(eventQueue.front());
        eventQueue.pop();
    }

    rotaryEncoder_.heartbeat();
}

void Input::configure()
{
    // Attach buttoninterrupts

    pinMode(BUTTON0_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON0_PIN), handleButton0Press, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1Press, HIGH);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2Press, HIGH);

    rotaryEncoder_.configure();
}

void Input::setCallback(InputHandlingCallback callback)
{
    inputHandlingCallback = callback;
}