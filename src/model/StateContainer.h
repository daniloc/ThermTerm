#ifndef SRC_MODEL_STATECONTAINER
#define SRC_MODEL_STATECONTAINER

#include "utility/SingletonTemplate.h"
#include "model/StateData.h"
#include "network/HAInterface.h"

#include "utility/Observation.h"

class StateContainer : public Subject<Observer, 5>
{
public:
    // constructors
    StateContainer();

    // getters
    StateData getState();
    HvacFanMode getFanSpeed() { return stateData_.fanSpeed; };

    bool shouldDimScreen();

    // setters
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setSetPoint(float setPoint);
    void setLux(float lux);
    void setHVACMode(HvacMode hvacMode);
    void setFanSpeed(HvacFanMode fanSpeed);
    void togglePower();

    void incrementSetPoint();
    void decrementSetPoint();

    void heartbeat();
    void configure();

    // non-static member functions
    void handleRemotePowerChange(bool powerState, HAHVAC *sender);
    void handleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender);
    void handleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender);
    void handleRemoteModeChange(HAHVAC::Mode, HAHVAC *);

    // static member functions
    static void staticHandleRemotePowerChange(bool powerState, HAHVAC *sender);
    static void staticHandleRemoteFanModeChange(HAHVAC::FanMode fanMode, HAHVAC *sender);
    static void staticHandleRemoteTargetTemperatureChange(HANumeric temperature, HAHVAC *sender);
    static void staticHandleRemoteModeChange(HAHVAC::Mode, HAHVAC *);

private:
    StateData stateData_;
    void updateMitsubishiInterface();
    HAInterface haInterface_;

    bool needsUpdate = false;
    void checkInputUpdate();

    static StateContainer *instance; // static pointer to store instance of StateContainer
};

#endif // SRC_MODEL_STATECONTAINER
