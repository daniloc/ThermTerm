#ifndef SRC_STATECONTAINER
#define SRC_STATECONTAINER

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
    StateContainer(float temperature, float humidity, float setPoint, HVACMode hvacMode, int fanSpeed);

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

private:
    float temperature_;
    float humidity_;
    float setPoint_;
    HVACMode hvacMode_;
    int fanSpeed_;
};

#endif // SRC_STATECONTAINER
