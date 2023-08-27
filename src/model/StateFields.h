#ifndef SRC_MODEL_STATEFIELDS
#define SRC_MODEL_STATEFIELDS

typedef enum HvacMode
{
    HVAC_HOT = 0,
    HVAC_COLD,
    HVAC_DRY,
    HVAC_FAN, // used for Panasonic only
    HVAC_AUTO
} HvacMode_t; // HVAC  MODE

typedef enum HvacPower
{
    OFF,
    ON
} HvacPower_t;

typedef enum HvacFanMode
{
    FAN_SPEED_AUTO = 0,
    FAN_SPEED_1,
    FAN_SPEED_2,
    FAN_SPEED_3,
    FAN_SPEED_4,
    FAN_SPEED_GAP,
    FAN_SPEED_SILENT = 6
} HvacFanMode_t; // HVAC  FAN MODE

typedef enum HvacVaneMode
{
    VANE_AUTO = 0,
    VANE_H1,
    VANE_H2,
    VANE_H3,
    VANE_H4,
    VANE_H5,
    VANE_AUTO_MOVE
} HvacVANEMode_t; // HVAC  VANE MODE

typedef enum HvacWideVANEMode
{
    WIDE_LEFT_END,
    WIDE_LEFT,
    WIDE_MIDDLE,
    WIDE_RIGHT,
    WIDE_RIGHT_END,
    WIDE_SWING
} HvacWideVANEMode_t; // HVAC  WIDE VANE MODE

typedef enum HvacAreaMode
{
    AREA_SWING,
    AREA_LEFT,
    AREA_AUTO,
    AREA_RIGHT
} HvacAreaMode_t; // HVAC  WIDE VANE MODE

#endif // SRC_MODEL_STATEFIELDS
