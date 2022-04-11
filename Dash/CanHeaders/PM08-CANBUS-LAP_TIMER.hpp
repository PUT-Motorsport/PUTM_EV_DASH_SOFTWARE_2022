//Generated on Thu Apr  7 17:02:54 2022
#ifndef LAP_TIMER
#define LAP_TIMER

#include <cstdint>

enum struct Lap_timer_states: uint8_t {
	Power_up,
	Pass_detected,
};

struct __attribute__ ((packed)) Lap_timer_Main{
	Lap_timer_states device_state; 
};

struct __attribute__ ((packed)) Lap_timer_StateChange{
	uint8_t Pass_detected; // 
};


const uint16_t LAP_TIMER_MAIN_CAN_ID = 0;
const uint8_t LAP_TIMER_MAIN_CAN_DLC = sizeof(Lap_timer_Main);
const uint8_t LAP_TIMER_MAIN_FREQUENCY = 5;
const uint16_t LAP_TIMER_STATECHANGE_CAN_ID = 0;
const uint8_t LAP_TIMER_STATECHANGE_CAN_DLC = sizeof(Lap_timer_StateChange);
const uint8_t LAP_TIMER_STATECHANGE_FREQUENCY = 0;

#endif

