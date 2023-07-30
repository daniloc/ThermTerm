#ifndef SRC_MODEL_STATEDATA
#define SRC_MODEL_STATEDATA

#include "hardware/MitsubishiInterface.h"

struct StateData
{
    float temperature = 0.0;
    float humidity = 0.0;
    float setPoint = 65;
    HvacMode hvacMode = HVAC_COLD;
    HvacFanMode fanSpeed = FAN_SPEED_5;
    HvacPower power = OFF;

    bool celsius = false;

    int heatPumpSetPoint() {
        if (celsius) {
            return int(setPoint);
        } else {
            return int((setPoint - 32) * 5.0 / 9.0);
        }
    }
};

#endif // SRC_MODEL_STATEDATA
