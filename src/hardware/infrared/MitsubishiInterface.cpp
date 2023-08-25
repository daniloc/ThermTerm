// Adapted from: https://github.com/r45635/HVAC-IR-Control/

#include "MitsubishiInterface.h"
#include "_Constants.h"

#define SEND_PWM_BY_TIMER
#include <IRremote.hpp>

const IRSignalTiming MitsubishiInterface::signalTiming = {
    .headerMark = 3400,
    .headerSpace = 1750,
    .bitMark = 450,
    .oneSpace = 1300,
    .zeroSpace = 420,
    .repeatMark = 440,
    .repeatSpace = 17100,
    .modulationFrequency = 38};

enum FieldIndices
{
  powerMode = 5,
  hvacMode = 6,
  temperatureSetPoint = 7,
  fanSettings = 9,
  crc = 17,
  frameLength = 18
};

void MitsubishiInterface::configure()
{
  IrSender.begin(INFRARED_PIN);
  IrSender.enableIROut(signalTiming.modulationFrequency);
}

void MitsubishiInterface::sendCommand(HVACCommand command)
{

  // #define HVAC_MITSUBISHI_DEBUG; // Un comment to access DEBUG information through Serial Interface

  byte mask = 1; // our bitmask
  byte frame[FieldIndices::frameLength] = {
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
      0x1F};
  // a valid frame; specific fields will be changed as needed
  byte i;

#ifdef HVAC_MITSUBISHI_DEBUG
  Serial.println("Packet to send: ");
  for (i = 0; i < 18; i++)
  {
    Serial.print("_");
    Serial.print(frame[i], HEX);
  }
  Serial.println(".");
#endif

  if (command.power == OFF)
  {
    frame[FieldIndices::powerMode] = (byte)0x0; // Turn OFF HVAC
  }
  else
  {
    frame[FieldIndices::powerMode] = (byte)0x20; // Tuen ON HVAC
  }

  static const byte hvacModeBytes[] = {
      0x08, // Corresponds to HVAC_HOT
      0x18, // Corresponds to HVAC_COLD
      0x10, // Corresponds to HVAC_DRY
      0x00, // Fan value?
      0x20 // Corresponds to HVAC_AUTO
  };

  if (command.hvacMode >= 0 && command.hvacMode < sizeof(hvacModeBytes))
  {
    frame[FieldIndices::hvacMode] = hvacModeBytes[command.hvacMode];
  }

  // Ensure the temperature set point is within acceptable range
  byte temperatureSetPoint;
  if (command.setPoint > HVAC_MAX_TEMP)
  {
    temperatureSetPoint = HVAC_MAX_TEMP;
  }
  else if (command.setPoint < HVAC_MIN_TEMP)
  {
    temperatureSetPoint = HVAC_MIN_TEMP;
  }
  else
  {
    temperatureSetPoint = command.setPoint;
  };

  frame[FieldIndices::temperatureSetPoint] = (byte)temperatureSetPoint - 16;

  static const byte fanSpeedLookup[] = {
      B00000001, // FAN_SPEED_1
      B00000010, // FAN_SPEED_2
      B00000011, // FAN_SPEED_3
      B00000100, // FAN_SPEED_4
      B00000100, // FAN_SPEED_5, same as speed 4
      B10000000, // FAN_SPEED_AUTO
      B00000101  // FAN_SPEED_SILENT
  };

  // Define a lookup table for VANE settings
  static const byte vaneModeLookup[] = {
      B01000000, // VANE_AUTO
      B01001000, // VANE_H1
      B01010000, // VANE_H2
      B01011000, // VANE_H3
      B01100000, // VANE_H4
      B01101000, // VANE_H5
      B01111000  // VANE_AUTO_MOVE
  };

  if (command.fanMode >= FAN_SPEED_1 && command.fanMode <= FAN_SPEED_SILENT)
  {
    frame[FieldIndices::fanSettings] = fanSpeedLookup[command.fanMode];
  }

  if (command.vaneMode >= VANE_AUTO && command.vaneMode <= VANE_AUTO_MOVE)
  {
    frame[FieldIndices::fanSettings] |= vaneModeLookup[command.vaneMode];
  }

  // Byte 18 - CRC
  frame[FieldIndices::crc] = 0;
  for (i = 0; i < 17; i++)
  {
    frame[FieldIndices::crc] = (byte)frame[i] + frame[FieldIndices::crc]; // CRC is a simple bits addition
  }

#ifdef HVAC_MITSUBISHI_DEBUG
  Serial.println("Packet to send: ");
  for (i = 0; i < 18; i++)
  {
    Serial.print("_");
    Serial.print(frame[i], HEX);
  }
  Serial.println(".");
  for (i = 0; i < 18; i++)
  {
    Serial.print(frame[i], BIN);
    Serial.print(" ");
  }
  Serial.println(".");
#endif

  for (int j = 0; j < 2; j++)
  { // For Mitsubishi IR protocol we send the frame twice
    // Header
    IrSender.mark(signalTiming.headerMark);
    IrSender.space(signalTiming.headerSpace);
    for (i = 0; i < FieldIndices::frameLength; i++)
    {
      // Send all Bits from Byte frame in Reverse Order
      for (mask = 0x01; mask > 0; mask <<= 1)
      { // iterate through bit mask
        if (frame[i] & mask)
        { // Bit ONE
          IrSender.mark(signalTiming.bitMark);
          IrSender.space(signalTiming.oneSpace);
        }
        else
        { // Bit ZERO
          IrSender.mark(signalTiming.bitMark);
          IrSender.space(signalTiming.zeroSpace);
        }
        // Next bits
      }
    }
    // End of Packet and retransmission of the Packet
    IrSender.mark(signalTiming.repeatMark);
    IrSender.space(signalTiming.repeatSpace);
  }
}
