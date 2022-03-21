//Generated on Sat Mar 19 12:16:05 2022
#ifndef APPS
#define APPS

#include <cstdint>

enum struct Apps_states: uint8_t {
	Power_up,
	Normal_operation,
	Sensor_Implausiblity,
};

struct __attribute__ ((packed)) Apps_main{
	uint16_t pedal_position; // pedal position form 0 – 1000 where 0 means pedal not pressed
	uint8_t counter; // counts from 0-255 to ensure all frames are recived
	int8_t position_diff; // pedal position derivative in [count / second]
    Apps_states device_state; // -----------------------------------------------------------------
};

const uint16_t APPS_MAIN_CAN_ID = 0;
const uint8_t APPS_MAIN_CAN_DLC = sizeof(Apps_main);

#endif

