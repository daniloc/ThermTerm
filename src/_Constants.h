#ifndef SRC_CONSTANTS
#define SRC_CONSTANTS

#include <Arduino.h>

#define INFRARED_PIN A0

#define HVAC_MAX_TEMP 31 // Celsius values
#define HVAC_MIN_TEMP 16

#define DIM_SCREEN_CUTOFF_LUX 5

#define PWM_TONE_CHANNEL 2 // Get off the main PWM channel, or LED pulses will leak out the speaker
#define PIEZO_PIN A1
#define PIEZO_BASE_FREQUENCY 200

#define UI_CHIRP_NOTE_LENGTH 25

#define SCREEN_WIDTH 240  // display width, in pixels
#define SCREEN_HEIGHT 135 // display height, in pixels

#define ENV_SENSOR_INTERVAL 5 // Seconds

const int BUTTON0_PIN = 0;
const int BUTTON1_PIN = 1;
const int BUTTON2_PIN = 2;

#define VERSION_NUMBER "0.2"

#define ROTARY_ENCODER_ADDRESS 0x36
#define ENCODER_SWITCH_PIN 24
#define ENCODER_NEOPIXEL_PIN 6

#endif // SRC_CONSTANTS
