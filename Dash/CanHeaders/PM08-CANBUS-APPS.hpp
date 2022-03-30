//Generated on Mon Mar 28 16:20:16 2022
#ifndef APPS
#define APPS

#include <cstdint>

enum struct Apps_states: uint8_t {
	Power_up,	// 0
	Normal_operation,	// 1
	Sensor_Implausiblity,	// 2
	Left_sensor_out_of_range___lower_bound,	// 3
	Left_sensor_out_of_range___upper_bound,	// 4
	Right_sensor_out_of_range___lower_bound,	// 5
	Right_sensor_out_of_range___upper_bound,	// 6
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

