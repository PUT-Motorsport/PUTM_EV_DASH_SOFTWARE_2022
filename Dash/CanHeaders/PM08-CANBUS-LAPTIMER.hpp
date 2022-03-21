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

const uint16_t LAP_TIMER_PASS_CAN_ID = 5;
const uint8_t LAP_TIMER_PASS_CAN_DLC = sizeof(Lap_timer_Pass);

#endif

