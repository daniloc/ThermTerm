// Adapted from: https://github.com/r45635/HVAC-IR-Control/

#include "MitsubishiInterface.h"
#include "_Constants.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Mitsubishi.h>

IRMitsubishiAC heatpump(INFRARED_PIN);

void MitsubishiInterface::configure()
{
  heatpump.begin();
}

void MitsubishiInterface::sendCommand(HVACCommand command)
{

  if (command.power == ON)
  {
    heatpump.on();
  }
  else
  {
    heatpump.off();
  }

  switch (command.hvacMode)
  {
  case HVAC_COLD:
    heatpump.setMode(kMitsubishiAcCool);
    break;
  case HVAC_HOT:
    heatpump.setMode(kMitsubishiAcHeat);
    break;
  case HVAC_DRY:
    heatpump.setMode(kMitsubishiAcDry);
    break;
  case HVAC_FAN:
    heatpump.setMode(kMitsubishiAcFan);
    break;
  case HVAC_AUTO:
    heatpump.setMode(kMitsubishiAcAuto);
  }

  heatpump.setFan(command.fanMode);
  heatpump.setTemp(command.setPoint);
  heatpump.send();

}
