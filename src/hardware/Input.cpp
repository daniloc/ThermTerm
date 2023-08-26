#include "Input.h"

#include <queue>

std::queue<UserInput> eventQueue;

int32_t encoder_position;

InputHandlingCallback Input::inputHandlingCallback = nullptr;

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;
bool lastButtonState = false;
bool buttonState = true;

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

// *** Lifecycle

void Input::heartbeat()
{

    //*** Rotary Encoder

    RotaryDirection rotaryDirection = readRotaryPosition();

    if (rotaryDirection == Up)
    {
        handleRotaryUp();
    }
    else if (rotaryDirection == Down)
    {
        handleRotaryDown();
    }

    //*** Encoder Button

    if (readRotaryButton() == true)
    {
        handleRotaryButton();
    }

    while (!eventQueue.empty())
    {
        inputHandlingCallback(eventQueue.front());
        eventQueue.pop();
    }
}

bool Input::readRotaryButton()
{

    bool buttonPressed = false;

    bool newButtonState = rotaryEncoder.digitalRead(ENCODER_SWITCH_PIN);

    if (newButtonState != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (newButtonState != buttonState)
        {
            buttonState = newButtonState;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == true)
            {
                buttonPressed = true;
            }
        }
    }

    lastButtonState = newButtonState;

    return buttonPressed;
}

RotaryDirection Input::readRotaryPosition()
{

    RotaryDirection direction = Unchanged;

    int32_t new_position = rotaryEncoder.getEncoderPosition();
    // did we move arounde?
    if (encoder_position != new_position && abs(new_position) % 2 == 0)
    {
        Serial.println(new_position); // display new position

        // change the neopixel color

        if (new_position < encoder_position)
        {
            direction = Up;
        }
        else
        {
            direction = Down;
        }

        encoder_position = new_position; // and save for next round
    }

    return direction;
}

void Input::configure()
{

    // Attach buttoninterrupts

    pinMode(BUTTON0_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON0_PIN), handleButton0Press, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1Press, HIGH);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2Press, HIGH);

    //***Rotary Encoder***

    Serial.print("Looking for seesaw!");

    if (!rotaryEncoder.begin(ROTARY_ENCODER_ADDRESS) || !encoderPixel_.begin(ROTARY_ENCODER_ADDRESS))
    {
        Serial.print("Couldn't find seesaw on default address");
    }
    else
    {
        Serial.print("seesaw started");
    }

    uint32_t version = ((rotaryEncoder.getVersion() >> 16) & 0xFFFF);
    if (version != 4991)
    {
        Serial.print("Wrong firmware loaded? ");
        Serial.print(version);
    }
    else
    {
        Serial.print("Found Product 4991");
    }

    // get starting position
    encoder_position = rotaryEncoder.getEncoderPosition();

    // use a pin for the built in encoder switch
    rotaryEncoder.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);

    rotaryEncoder.setGPIOInterrupts((uint32_t)1 << ENCODER_SWITCH_PIN, 1);
    rotaryEncoder.enableEncoderInterrupt();
}

void Input::setCallback(InputHandlingCallback callback)
{
    inputHandlingCallback = callback;
}