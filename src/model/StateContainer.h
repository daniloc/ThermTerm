#ifndef SRC_MODEL_STATECONTAINER
#define SRC_MODEL_STATECONTAINER

class HAIntegration;

#include "utility/SingletonTemplate.h"

enum class HVACMode
{
    OFF,
    FAN,
    COOL,
    HOT
};

class StateContainer
{
public:
    // constructors
    StateContainer();

    // getters
    float getTemperature() const;
    float getHumidity() const;
    float getSetPoint() const;
    HVACMode getHVACMode() const;
    int getFanSpeed() const;

    // setters
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setSetPoint(float setPoint);
    void setHVACMode(HVACMode hvacMode);
    void setFanSpeed(int fanSpeed);

    void incrementSetPoint();
    void decrementSetPoint();

    void setHAIntegration(HAIntegration *haIntegration);

    void heartbeat();
    void configure();

private:
    float temperature_;
    float humidity_;
    float setPoint_;
    HVACMode hvacMode_;
    int fanSpeed_;
    void updateMitsubishiInterface();
};

#endif // SRC_MODEL_STATECONTAINER
