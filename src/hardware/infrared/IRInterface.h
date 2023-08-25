#ifndef SRC_HARDWARE_INFRARED_IRINTERFACE
#define SRC_HARDWARE_INFRARED_IRINTERFACE

#include "model/SystemState.h"

struct IRSignalTiming
{
    int headerMark;
    int headerSpace;
    int bitMark;
    int oneSpace;
    int zeroSpace;
    int repeatMark;
    int repeatSpace;
    int modulationFrequency;
};

class IRInterface
{
public:
    virtual ~IRInterface() = default;
    virtual void configure();
    virtual void sendCommand(HVACCommand);
};

#endif // SRC_HARDWARE_INFRARED_IRINTERFACE
