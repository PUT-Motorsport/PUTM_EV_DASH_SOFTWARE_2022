//Generated on Thu Apr  7 17:02:54 2022
#ifndef DASH
#define DASH

#include <cstdint>

enum struct Dash_states: uint8_t {
	Power_up,
	Normal_operation,
	Driverless,
};

struct __attribute__ ((packed)) Dash_Main{
	Dash_states device_state; 
};

struct __attribute__ ((packed)) Dash_TCS_frame{
    uint8_t on; // TC- ON - OFF
    uint8_t max_slip_ratio; // 0-100%
    uint8_t algorithm; // "PI (PID
    uint8_t max_power; // 0-100%
    uint8_t apps_curve; // "0 - Linear
    uint8_t regen_power; // 0-100%
    uint8_t sensivity; // 0-100% [100% - no wheel slip]
    uint8_t CRC;
};

struct __attribute__ ((packed)) Dash_Smart_Fuses_FAN_speed{
    uint8_t fan_2; // 0-100% []
    uint8_t water_pump; // on off
};

const uint16_t DASH_MAIN_CAN_ID = 0;
const uint8_t DASH_MAIN_CAN_DLC = sizeof(Dash_Main);
const uint8_t DASH_MAIN_FREQUENCY = 10;

const uint16_t DASH_TCS_FRAME_CAN_ID = 3;
const uint8_t DASH_TCS_FRAME_CAN_DLC = sizeof(Dash_TCS_frame);
const uint8_t DASH_TCS_FRAME_FREQUENCY = 0;

const uint16_t DASH_SMART_FUSES_FAN_SPEED_CAN_ID = 0;
const uint8_t DASH_SMART_FUSES_FAN_SPEED_CAN_DLC = sizeof(Dash_Smart_Fuses_FAN_speed);
const uint8_t DASH_SMART_FUSES_FAN_SPEED_FREQUENCY = 0;



#endif

