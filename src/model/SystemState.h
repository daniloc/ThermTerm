#ifndef SRC_MODEL_STATEDATA
#define SRC_MODEL_STATEDATA

#include "StateFields.h"

struct HVACCommand
{
    HvacMode hvacMode;
    int setPoint;
    HvacFanMode fanMode;
    HvacVaneMode vaneMode;
    HvacPower power;
};

struct SystemState
{
    float temperature = 0.0;
    float humidity = 0.0;
    float setPoint = 65;
    float lux = 0;
    HvacMode hvacMode = HVAC_COLD;
    HvacFanMode fanSpeed = FAN_SPEED_5;
    HvacPower power = OFF;

    bool celsius = false;

    int heatPumpSetPoint()
    {
        if (celsius)
        {
            return int(setPoint);
        }
        else
        {
            return int((setPoint - 32) * 5.0 / 9.0);
        }
    }

    HVACCommand hvacCommand() const
    {
        HVACCommand command;
        command.hvacMode = hvacMode;
        command.setPoint = setPoint;
        command.fanMode = fanSpeed;
        command.vaneMode = VANE_AUTO;
        command.power = power;
        return command;
    }
};

#endif // SRC_MODEL_STATEDATA
