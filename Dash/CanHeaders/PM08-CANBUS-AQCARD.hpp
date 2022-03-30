//Generated on Mon Mar 28 16:20:12 2022
#ifndef AQ
#define AQ

#include <cstdint>

enum struct AQ_states: uint8_t {
	Power_up,
	Normal_operation,
	Sensor_impossibility,
};

struct __attribute__ ((packed)) AQ_main{
	uint16_t wheel_speed_right; // rotation wheel speed rpm
	uint16_t wheel_speed_left; // rotation wheel speed rpm
	uint16_t break_pressure; // pressure of braking lquid front
	AQ_states device_state; // -----------------------------------------------------------------
};

struct __attribute__ ((packed)) AQ_name{
	uint16_t air_flow_velocity; // rotation wheel speed rpm
};

const uint16_t AQ_MAIN_CAN_ID = 0;
const uint8_t AQ_MAIN_CAN_DLC = sizeof(AQ_main);
const uint16_t AQ_NAME_CAN_ID = 0;
const uint8_t AQ_NAME_CAN_DLC = sizeof(AQ_name);

#endif

