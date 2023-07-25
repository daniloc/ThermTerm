#include "MitsubishiInterface.h"
#include <IRremote.hpp>

void MitsubishiInterface::prepare() {
  IrSender.begin(A0);
  IrSender.enableIROut(38);
}
/****************************************************************************
/* Send IR command to Mitsubishi HVAC - sendHvacMitsubishi
/***************************************************************************/
void MitsubishiInterface::sendHvacMitsubishi(
  HvacMode HVAC_Mode, // Example HVAC_HOT  HvacMitsubishiMode
  int HVAC_Temp, // Example 21  (°c)
  HvacFanMode HVAC_FanMode, // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
  HvacVanneMode HVAC_VanneMode, // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
  HvacPower POWER // Example false
) {

  //#define HVAC_MITSUBISHI_DEBUG; // Un comment to access DEBUG information through Serial Interface

  byte mask = 1; //our bitmask
  byte data[18] = {
    0x23,
    0xCB,
    0x26,
    0x01,
    0x00,
    0x20,
    0x08,
    0x06,
    0x30,
    0x45,
    0x67,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F
  };
  // data array is a valid trame, only byte to be chnaged will be updated.

  byte i;

  #ifdef HVAC_MITSUBISHI_DEBUG
  //Serial.println("Packet to send: ");
  for (i = 0; i < 18; i++) {
    //Serial.print("_");
    //Serial.print(data[i], HEX);
  }
  //Serial.println(".");
  #endif

  // Byte 6 - On / Off
  if (POWER == OFF) {
    data[5] = (byte) 0x0; // Turn OFF HVAC
  } else {
    data[5] = (byte) 0x20; // Tuen ON HVAC
  }

  // Byte 7 - Mode
  switch (HVAC_Mode) {
  case HVAC_HOT:
    data[6] = (byte) 0x08;
    break;
  case HVAC_COLD:
    data[6] = (byte) 0x18;
    break;
  case HVAC_DRY:
    data[6] = (byte) 0x10;
    break;
  case HVAC_AUTO:
    data[6] = (byte) 0x20;
    break;
  default:
    break;
  }

  // Byte 8 - Temperature
  // Check Min Max For Hot Mode
  byte Temp;
  if (HVAC_Temp > 31) {
    Temp = 31;
  } else if (HVAC_Temp < 16) {
    Temp = 16;
  } else {
    Temp = HVAC_Temp;
  };
  data[7] = (byte) Temp - 16;

  // Byte 10 - FAN / VANNE
  switch (HVAC_FanMode) {
  case FAN_SPEED_1:
    data[9] = (byte) B00000001;
    break;
  case FAN_SPEED_2:
    data[9] = (byte) B00000010;
    break;
  case FAN_SPEED_3:
    data[9] = (byte) B00000011;
    break;
  case FAN_SPEED_4:
    data[9] = (byte) B00000100;
    break;
  case FAN_SPEED_5:
    data[9] = (byte) B00000100;
    break; //No FAN speed 5 for MITSUBISHI so it is consider as Speed 4
  case FAN_SPEED_AUTO:
    data[9] = (byte) B10000000;
    break;
  case FAN_SPEED_SILENT:
    data[9] = (byte) B00000101;
    break;
  default:
    break;
  }

  switch (HVAC_VanneMode) {
  case VANNE_AUTO:
    data[9] = (byte) data[9] | B01000000;
    break;
  case VANNE_H1:
    data[9] = (byte) data[9] | B01001000;
    break;
  case VANNE_H2:
    data[9] = (byte) data[9] | B01010000;
    break;
  case VANNE_H3:
    data[9] = (byte) data[9] | B01011000;
    break;
  case VANNE_H4:
    data[9] = (byte) data[9] | B01100000;
    break;
  case VANNE_H5:
    data[9] = (byte) data[9] | B01101000;
    break;
  case VANNE_AUTO_MOVE:
    data[9] = (byte) data[9] | B01111000;
    break;
  default:
    break;
  }

  // Byte 18 - CRC
  data[17] = 0;
  for (i = 0; i < 17; i++) {
    data[17] = (byte) data[i] + data[17]; // CRC is a simple bits addition
  }

  #ifdef HVAC_MITSUBISHI_DEBUG
  Serial.println("Packet to send: ");
  for (i = 0; i < 18; i++) {
    Serial.print("_");
    Serial.print(data[i], HEX);
  }
  Serial.println(".");
  for (i = 0; i < 18; i++) {
    Serial.print(data[i], BIN);
    Serial.print(" ");
  }
  Serial.println(".");
  #endif

  IrSender.space(0);

  for (int j = 0; j < 2; j++) { // For Mitsubishi IR protocol we have to send two time the packet data
    // Header for the Packet
    IrSender.mark(HVAC_MITSUBISHI_HDR_MARK);
    IrSender.space(HVAC_MITSUBISHI_HDR_SPACE);
    for (i = 0; i < 18; i++) {
      // Send all Bits from Byte Data in Reverse Order
      for (mask = 00000001; mask > 0; mask <<= 1) { //iterate through bit mask
        if (data[i] & mask) { // Bit ONE
          IrSender.mark(HVAC_MITSUBISHI_BIT_MARK);
          IrSender.space(HVAC_MITSUBISHI_ONE_SPACE);
        } else { // Bit ZERO
          IrSender.mark(HVAC_MITSUBISHI_BIT_MARK);
          IrSender.space(HVAC_MISTUBISHI_ZERO_SPACE);
        }
        //Next bits
      }
    }
    // End of Packet and retransmission of the Packet
    IrSender.mark(HVAC_MITSUBISHI_RPT_MARK);
    IrSender.space(HVAC_MITSUBISHI_RPT_SPACE);
    IrSender.space(0); // Just to be sure
  }
}
