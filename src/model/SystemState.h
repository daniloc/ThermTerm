#ifndef SRC_MODEL_STATEDATA
#define SRC_MODEL_STATEDATA

#include "StateFields.h"

struct HVACCommand
{
    HvacMode hvacMode;
    float setPoint;
    HvacFanMode fanMode;
    HvacVaneMode vaneMode;
    HvacPower power;
};

struct SystemState
{
    float temperature = 0.0;
    float humidity = 0.0;
    float setPointC = 18;
    float lux = 0;
    HvacMode hvacMode = HVAC_COLD;
    HvacFanMode fanSpeed = FAN_SPEED_4;
    HvacPower power = OFF;

    bool celsius = false;

    float getSetPointUIValue()
    {
        if (celsius)
        {
            return setPointC;
        }
        else
        {
            return setPointC * 9.0 / 5.0 + 32;
        }
    }

    void updateSetPoint(float newSetPoint)
    {
        if (celsius)
        {
            setPointC = newSetPoint;
        }
        else
        {
            setPointC = (newSetPoint - 32) * 5.0 / 9.0;
        }
    }

    HVACCommand hvacCommand() const
    {
        HVACCommand command;
        command.hvacMode = hvacMode;
        command.setPoint = setPointC;
        command.fanMode = fanSpeed;
        command.vaneMode = VANE_AUTO;
        command.power = power;
        return command;
    }
};

#endif // SRC_MODEL_STATEDATA
