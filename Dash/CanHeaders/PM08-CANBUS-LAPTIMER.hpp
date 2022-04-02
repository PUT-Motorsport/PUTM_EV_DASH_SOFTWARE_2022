//Generated on Sat Mar 19 12:16:05 2022
#ifndef LAP_TIMER
#define LAP_TIMER

#include <cstdint>

enum struct Lap_timer_states: uint8_t {
	Waiting,
	Pass_detected,
};

struct __attribute__ ((packed)) Lap_timer_Pass{
    Lap_timer_states device_state; // --------------------------------
};

struct __attribute__ ((packed)) Lap_timer_StateChange{
    uint8_t Pass_detected; //
};

const uint16_t LAP_TIMER_PASS_CAN_ID = 5;
const uint8_t LAP_TIMER_PASS_CAN_DLC = sizeof(Lap_timer_Pass);
const uint8_t LAP_TIMER_MAIN_FREQUENCY = 5;
const uint16_t LAP_TIMER_STATECHANGE_CAN_ID = 0;
const uint8_t LAP_TIMER_STATECHANGE_CAN_DLC = sizeof(Lap_timer_StateChange);
const uint8_t LAP_TIMER_STATECHANGE_FREQUENCY = 0;
#endif
