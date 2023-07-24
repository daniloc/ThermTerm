#include "Input.h"
#include "Adafruit_seesaw.h"
#include "seesaw_neopixel.h"

#define ENCODER_SWITCH 24
#define ENCODER_NEOPIXEL 6

#define SEESAW_ADDR 0x36

Adafruit_seesaw rotaryEncoder;
seesaw_NeoPixel encoderPixel = seesaw_NeoPixel(1, ENCODER_NEOPIXEL, NEO_GRB + NEO_KHZ800);
int32_t encoder_position;

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;
bool lastButtonState = false;
bool buttonState = true;

#define BUTTON0_PIN 0
#define BUTTON1_PIN 1
#define BUTTON2_PIN 2

volatile unsigned long lastInterruptTime0 = 0;
volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;
unsigned long debounceTime = 500; // Debounce time in milliseconds

void handleButton0Press()
{
    if ((millis() - lastInterruptTime0) > debounceTime)
    {
        Serial.println(F("Button 0 pressed"));
        
    }
    lastInterruptTime0 = millis();
}

void handleButton1Press()
{
    if ((millis() - lastInterruptTime1) > debounceTime)
    {
        Serial.println(F("Button 1 pressed"));
    }
    lastInterruptTime1 = millis();
}

void handleButton2Press()
{
    if ((millis() - lastInterruptTime2) > debounceTime)
    {
        Serial.println(F("Button 2 pressed"));
    }
    lastInterruptTime2 = millis();
}

uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    return 0;
}

// *** Input Handlers

void Input::handleRotaryButton()
{
    Serial.print(F("Rotary button"));
}

void Input::handleRotaryUp()
{
    Serial.print(F("Rotary up"));
}

void Input::handleRotaryDown()
{
    Serial.print(F("Rotary down"));
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
}

bool Input::readRotaryButton()
{

    bool buttonPressed = false;

    bool newButtonState = rotaryEncoder.digitalRead(ENCODER_SWITCH);

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
    if (encoder_position != new_position)
    {
        Serial.println(new_position); // display new position

        // change the neopixel color
        encoderPixel.setPixelColor(0, Wheel(new_position & 0xFF));
        encoderPixel.show();

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

    //***Rotary Encoder***

    Serial.println("Looking for seesaw!");

    if (!rotaryEncoder.begin(SEESAW_ADDR) || !encoderPixel.begin(SEESAW_ADDR))
    {
        Serial.println("Couldn't find seesaw on default address");
    }
    else
    {
        Serial.println("seesaw started");
    }

    uint32_t version = ((rotaryEncoder.getVersion() >> 16) & 0xFFFF);
    if (version != 4991)
    {
        Serial.print("Wrong firmware loaded? ");
        Serial.println(version);
    }
    else
    {
        Serial.println("Found Product 4991");
    }

    // set not so bright!
    encoderPixel.setBrightness(20);
    encoderPixel.show();

    // get starting position
    encoder_position = rotaryEncoder.getEncoderPosition();

    // use a pin for the built in encoder switch
    rotaryEncoder.pinMode(ENCODER_SWITCH, INPUT_PULLUP);

    rotaryEncoder.setGPIOInterrupts((uint32_t)1 << ENCODER_SWITCH, 1);
    rotaryEncoder.enableEncoderInterrupt();

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(BUTTON0_PIN), handleButton0Press, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1Press, HIGH);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2Press, HIGH);
}