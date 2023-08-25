#ifndef SRC_HARDWARE_SOUNDINTERFACE
#define SRC_HARDWARE_SOUNDINTERFACE

#include <Arduino.h>

#define PWM_TONE_CHANNEL 2 // Get off the main PWM channel, or LED pulses will leak out the speaker
#define PIEZO_PIN A1

#define PIEZO_BASE_FREQUENCY 200

#define UI_CHIRP_NOTE_LENGTH 25

class Audio
{
public:
    static void configure()
    {
        setToneChannel(PWM_TONE_CHANNEL);
    }

    static void play_UpwardTone()
    {
        tone(PIEZO_PIN, PIEZO_BASE_FREQUENCY, UI_CHIRP_NOTE_LENGTH);
        tone(PIEZO_PIN, PIEZO_BASE_FREQUENCY * 1.25, UI_CHIRP_NOTE_LENGTH * 2);
    }

    static void play_DownwardTone()
    {
        tone(PIEZO_PIN, PIEZO_BASE_FREQUENCY, UI_CHIRP_NOTE_LENGTH);
        tone(PIEZO_PIN, PIEZO_BASE_FREQUENCY * 0.75, UI_CHIRP_NOTE_LENGTH * 2);
    }
};

#endif // SRC_HARDWARE_SOUNDINTERFACE
