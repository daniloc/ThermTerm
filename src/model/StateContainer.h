#ifndef SRC_MODEL_STATECONTAINER
#define SRC_MODEL_STATECONTAINER

#include "utility/SingletonTemplate.h"
#include "model/StateData.h"
#include "network/HAInterface.h"


class StateContainer
{
public:
    // constructors
    StateContainer();

    // getters
    StateData getState();

    // setters
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setSetPoint(float setPoint);
    void setHVACMode(HvacMode hvacMode);
    void setFanSpeed(HvacFanMode fanSpeed);

    void incrementSetPoint();
    void decrementSetPoint();

    void heartbeat();
    void configure();

private:
    StateData stateData_;
    void updateMitsubishiInterface();
    HAInterface haInterface_;

};

#endif // SRC_MODEL_STATECONTAINER
