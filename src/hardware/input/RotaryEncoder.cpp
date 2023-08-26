#include "RotaryEncoder.h"
#include "Input.h"

void RotaryEncoder::configure()
{
    Serial.print("Looking for seesaw!");

    if (!seesawInterface_.begin(ROTARY_ENCODER_ADDRESS) || !neoPixel_.begin(ROTARY_ENCODER_ADDRESS))
    {
        Serial.print("Couldn't find seesaw on default address");
    }
    else
    {
        Serial.print("seesaw started");
    }

    uint32_t version = ((seesawInterface_.getVersion() >> 16) & 0xFFFF);
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
    encoderPosition = seesawInterface_.getEncoderPosition();

    // use a pin for the built in encoder switch
    seesawInterface_.pinMode(ENCODER_SWITCH_PIN, INPUT_PULLUP);

    seesawInterface_.setGPIOInterrupts((uint32_t)1 << ENCODER_SWITCH_PIN, 1);
    seesawInterface_.enableEncoderInterrupt();
}

void RotaryEncoder::heartbeat() {

    Direction rotaryDirection = readRotaryPosition();

    if (rotaryDirection != Unchanged) {
        if (rotaryDirection == Right) {
            Input::shared().rotaryEncoderHandler(Up);
        } else {
            Input::shared().rotaryEncoderHandler(Down);
        }
    }

    if (readButton() == true) {
        Input::shared().rotaryEncoderHandler(Clicked);
    }
}

RotaryEncoder::Direction RotaryEncoder::readRotaryPosition()
{
    Direction direction = Unchanged;

    int32_t newPosition = seesawInterface_.getEncoderPosition();
    if (encoderPosition != newPosition && abs(newPosition) % 2 == 0)
    {
        Serial.println(newPosition); // display new position

        // change the neopixel color

        if (newPosition < encoderPosition)
        {
            direction = Right;
        }
        else
        {
            direction = Left;
        }

        encoderPosition = newPosition; // and save for next round
    }

    return direction;
}

bool RotaryEncoder::readButton()
{

    bool buttonPressed = false;

    bool newButtonState = seesawInterface_.digitalRead(ENCODER_SWITCH_PIN);

    if (newButtonState != lastButtonState)
    {
        // reset the debouncing timer
        lastButtonDebounceTime = millis();
    }

    if ((millis() - lastButtonDebounceTime) > debounceInterval)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (newButtonState != buttonState)
        {
            buttonState = newButtonState;

            if (buttonState == true)
            {
                buttonPressed = true;
            }
        }
    }

    lastButtonState = newButtonState;

    return buttonPressed;
}