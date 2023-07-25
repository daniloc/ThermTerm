/*
 * IRremote
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 *
 * Mitsubishi HVAC protocol added by Vincent Cruvellier.
 * Mitsubishi HVAC W001CP R61Y23304 protocol added by bt4wang.
 */

#ifndef SRC_HARDWARE_MITSUBISHIINTERFACE
#define SRC_HARDWARE_MITSUBISHIINTERFACE

#define HVAC_MITSUBISHI_HDR_MARK 3400
#define HVAC_MITSUBISHI_HDR_SPACE 1750
#define HVAC_MITSUBISHI_BIT_MARK 450
#define HVAC_MITSUBISHI_ONE_SPACE 1300
#define HVAC_MISTUBISHI_ZERO_SPACE 420
#define HVAC_MITSUBISHI_RPT_MARK 440
#define HVAC_MITSUBISHI_RPT_SPACE 17100 // Above original iremote limit

typedef enum HvacMode
{
  HVAC_HOT,
  HVAC_COLD,
  HVAC_DRY,
  HVAC_FAN, // used for Panasonic only
  HVAC_AUTO
} HvacMode_t; // HVAC  MODE

typedef enum HvacPower
{
  OFF,
  ON
};

typedef enum HvacFanMode
{
  FAN_SPEED_1,
  FAN_SPEED_2,
  FAN_SPEED_3,
  FAN_SPEED_4,
  FAN_SPEED_5,
  FAN_SPEED_AUTO,
  FAN_SPEED_SILENT
} HvacFanMode_; // HVAC  FAN MODE

typedef enum HvacVanneMode
{
  VANNE_AUTO,
  VANNE_H1,
  VANNE_H2,
  VANNE_H3,
  VANNE_H4,
  VANNE_H5,
  VANNE_AUTO_MOVE
} HvacVanneMode_; // HVAC  VANNE MODE

typedef enum HvacWideVanneMode
{
  WIDE_LEFT_END,
  WIDE_LEFT,
  WIDE_MIDDLE,
  WIDE_RIGHT,
  WIDE_RIGHT_END,
  WIDE_SWING
} HvacWideVanneMode_t; // HVAC  WIDE VANNE MODE

typedef enum HvacAreaMode
{
  AREA_SWING,
  AREA_LEFT,
  AREA_AUTO,
  AREA_RIGHT
} HvacAreaMode_t; // HVAC  WIDE VANNE MODE

class MitsubishiInterface
{

public:
  MitsubishiInterface() {}
  void sendHvacMitsubishi(
      HvacMode HVAC_Mode,           // Example HVAC_HOT  HvacMitsubishiMode
      int HVAC_Temp,                // Example 21  (°c)
      HvacFanMode HVAC_FanMode,     // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
      HvacVanneMode HVAC_VanneMode, // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
      HvacPower POWER               // Example false
  );

  void prepare();

};

#endif
