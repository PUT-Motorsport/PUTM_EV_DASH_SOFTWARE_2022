//Generated on Sun Nov 13 12:09:33 2022
#ifndef LAP_TIMER
#define LAP_TIMER

#include <cstdint>

namespace PUTM_CAN {
enum struct Lap_timer_states: uint8_t {
    Normal_operation,	//
    Startfinish_battery_low,	// Warning
    Sector_2_battery_low,	// Warning
    Sector_3_battery_low,	// Warning
    Missed_detection,	// Warning
    Timeout,	// Warning
    Error,	//
};

struct __attribute__ ((packed)) Lap_timer_Main{
    Lap_timer_states device_state; //
};

struct __attribute__ ((packed)) Lap_timer_Sector{
    uint8_t Sector; // "0x00 - default
    uint32_t  Sector_time; // In miliseconds
    Lap_timer_states device_state; //
};

struct __attribute__ ((packed)) Lap_timer_Acc_time{
    uint32_t Acc_Time; // In miliseconds
    Lap_timer_states device_state; //
};

struct __attribute__ ((packed)) Lap_timer_Skidpad_time{
    uint32_t Skidpad_time; // In miliseconds
    Lap_timer_states device_state; //
};

struct __attribute__ ((packed)) Lap_timer_Lap_time{
    uint32_t Lap_time; // In miliseconds
    Lap_timer_states device_state; //
};


const uint16_t LAP_TIMER_MAIN_CAN_ID = 0x78;
const uint8_t LAP_TIMER_MAIN_CAN_DLC = sizeof(Lap_timer_Main);
const uint8_t LAP_TIMER_MAIN_FREQUENCY = 5;
const uint16_t LAP_TIMER_SECTOR_CAN_ID = 0x3c;
const uint8_t LAP_TIMER_SECTOR_CAN_DLC = sizeof(Lap_timer_Sector);
const uint8_t LAP_TIMER_SECTOR_FREQUENCY = 0;
const uint16_t LAP_TIMER_ACC_TIME_CAN_ID = 0x91;
const uint8_t LAP_TIMER_ACC_TIME_CAN_DLC = sizeof(Lap_timer_Acc_time);
const uint8_t LAP_TIMER_ACC_TIME_FREQUENCY = 0;
const uint16_t LAP_TIMER_SKIDPAD_TIME_CAN_ID = 0x96;
const uint8_t LAP_TIMER_SKIDPAD_TIME_CAN_DLC = sizeof(Lap_timer_Skidpad_time);
const uint8_t LAP_TIMER_SKIDPAD_TIME_FREQUENCY = 0;
const uint16_t LAP_TIMER_LAP_TIME_CAN_ID = 0x9b;
const uint8_t LAP_TIMER_LAP_TIME_CAN_DLC = sizeof(Lap_timer_Lap_time);
const uint8_t LAP_TIMER_LAP_TIME_FREQUENCY = 0;

} // namespace can

#endif
