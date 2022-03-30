//Generated on Mon Mar 21 18:53:53 2022
#ifndef TS
#define TS

#include <cstdint>

enum struct TS_states: uint8_t {
	OK,
	Power_up,
	TS_OFF,
	TS_ON,
	APPS_TIMEOUT,
	APPS_INVALID_VALUE,
	APPS_SKIP_FRAME,
	INV_TIMEOUT,
	INV_IPEAK,
};

struct __attribute__ ((packed)) TS_main{
	uint16_t vehicle_speed; // Estimated vehicle velocity [km/h * 100]
	uint8_t water_temp; // Water temperature at the radiator input [deg C]
	uint8_t water_pressure; // todo
	int8_t motor_current; // positive for normal operation negative for regen [A]
	bool rtds_active: 1; // rtds is making sound
	bool brake_light_active: 1; 
	bool tc_enable: 1; 
	bool tc_active: 1; 
	bool regen_enable: 1; 
	bool regen_active: 1; 
	TS_states device_state; // -------------------------
};

struct __attribute__ ((packed)) TS_rear_suspension{
	uint16_t adc_susp_right; 
	uint16_t adc_susp_left; 
	int16_t acc_lateral; 
	int16_t acc_longitunal; 
};

const uint16_t TS_MAIN_CAN_ID = 0;
const uint8_t TS_MAIN_CAN_DLC = sizeof(TS_main);
const uint16_t TS_REAR_SUSPENSION_CAN_ID = 0;
const uint8_t TS_REAR_SUSPENSION_CAN_DLC = sizeof(TS_rear_suspension);

#endif

