//Generated on Sat Apr 30 12:45:17 2022
#ifndef BMS_LV
#define BMS_LV

#include <cstdint>

enum struct BMS_LV_states: uint8_t {
	Normal,
	Charging,	// warning
	Unbalanced,	// warning
	Voltage_low,	// shut down
	Voltage_high,	// shut down
	Temp_high,	// shut down
	Current_high,	// shut down
	Sleep,	// shut down
};

struct __attribute__ ((packed)) BMS_LV_main{
	int16_t voltage_sum; // eg 2500 means 25.00V
	int8_t soc; // state of charge
	int8_t temp_avg; // in Celsius
	int8_t current; 
	BMS_LV_states device_state; 
};

struct __attribute__ ((packed)) BMS_LV_temperature{
	int8_t temp_1; // in Celsius
	int8_t temp_2; // in Celsius
	int8_t temp_3; // in Celsius
	int8_t temp_4; // in Celsius
	int8_t temp_5; // in Celsius
	int8_t temp_6; // in Celsius
	int8_t temp_7; // in Celsius
	int8_t temp_8; // in Celsius
};


const uint16_t BMS_LV_MAIN_CAN_ID = 0xf;
const uint8_t BMS_LV_MAIN_CAN_DLC = sizeof(BMS_LV_main);
const uint8_t BMS_LV_MAIN_FREQUENCY = 100;
const uint16_t BMS_LV_TEMPERATURE_CAN_ID = 0x14;
const uint8_t BMS_LV_TEMPERATURE_CAN_DLC = sizeof(BMS_LV_temperature);
const uint8_t BMS_LV_TEMPERATURE_FREQUENCY = 1;

#endif

