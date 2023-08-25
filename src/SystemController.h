#ifndef SRC_SYSTEMCONTROLLER
#define SRC_SYSTEMCONTROLLER

#include "utility/SingletonTemplate.h"
#include "model/SystemState.h"
#include "network/HAInterface.h"
#include "utility/Observation.h"
#include "hardware/infrared/IRInterface.h"

class SystemController : public Subject<Observer, 5>
{
public:
    // constructors
    SystemController(IRInterface &);

    // getters
    SystemState getState();
    HvacFanMode getFanSpeed() { return state_.fanSpeed; };

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
    static void staticHandleAlert(bool status, HASwitch *sender);

private:
    SystemState state_;
    void sendInfraredCommand();
    HAInterface haInterface_;
    IRInterface &irInterface_;

    bool needsUpdate = false;
    void checkInputBatching();
    void scheduleUpdate();
    unsigned long batchScheduleTime = 0;

    static SystemController *instance; // static pointer to store instance of SystemController
};

#endif // SRC_SYSTEMCONTROLLER
