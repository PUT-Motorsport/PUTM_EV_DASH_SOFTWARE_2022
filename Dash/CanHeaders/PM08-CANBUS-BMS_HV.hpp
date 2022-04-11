//Generated on Thu Apr  7 17:02:54 2022
#ifndef BMS_HV
#define BMS_HV

#include <cstdint>

enum struct BMS_HV_states: uint8_t {
	AIR_opened,	// normal
	AIR_closed,	// normal
	Precharge,	// normal
	Charger_connected,	// normal
	Unbalanced,	// warning
	Unbalanced___critical,	// shut down
	Voltage_low,	// shut down
	Voltage_high,	// shut down
	Temp_high,	// shut down
	Current_high,	// shut down
};

struct __attribute__ ((packed)) BMS_HV_main{
	int16_t voltage_sum; 
	int8_t soc; // state of charge
	int8_t temp_max; 
	int8_t temp_avg; // in Celsius
	int8_t current; 
	BMS_HV_states device_state; 
};


const uint16_t BMS_HV_MAIN_CAN_ID = 0;
const uint8_t BMS_HV_MAIN_CAN_DLC = sizeof(BMS_HV_main);
const uint8_t BMS_HV_MAIN_FREQUENCY = 100;

#endif

