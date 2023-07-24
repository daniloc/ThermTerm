#ifndef SRC_HARDWARE_ENVSENSOR
#define SRC_HARDWARE_ENVSENSOR


struct EnvData {
  float temp;
  float humidity;
};

typedef void (*EnvSensorCallback)(float temp, float humidity);

class EnvSensor {
    public:
    void configure();
    void heartbeat();
    void setCallback(EnvSensorCallback callback);

  private:
    static EnvData update();
    static bool timerEvent(void *argument);

    static EnvSensorCallback envDataUpdateFunction;
};

#endif // SRC_HARDWARE_ENVSENSOR
