#ifndef SRC_HARDWARE_INFRARED_MITSUBISHIINTERFACE
#define SRC_HARDWARE_INFRARED_MITSUBISHIINTERFACE

#include "IRInterface.h"

class MitsubishiInterface : public IRInterface
{

public:
  static const IRSignalTiming signalTiming;

  MitsubishiInterface() {}

  void sendCommand(HVACCommand) override;
  void configure();
};

#endif // SRC_HARDWARE_INFRARED_MITSUBISHIINTERFACE
