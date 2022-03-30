//Generated on Mon Mar 28 16:20:17 2022
#ifndef STEERING_WHEEL
#define STEERING_WHEEL

#include <cstdint>

enum struct Steering_Wheel_states: uint8_t {
	OK,
	Power_up,
	Normal_operation,
};

struct __attribute__ ((packed)) Steering_Wheel_main{
	uint8_t s_w_a; // "steering wheel angle [-180
	Steering_Wheel_states device_state; // -----------------------------------------------------------------
};

struct __attribute__ ((packed)) Steering_Wheel_Name{
	uint8_t button1; 
	uint8_t button2; 
	uint8_t button3; 
	uint8_t button4; 
	uint8_t button5; // button1+button2
	uint8_t button6; // button1+button3
	uint8_t button7; // button1+button4
	uint8_t button8; // button2+button3
	uint8_t button9; // button2+button4
	uint8_t button10; // button3+button4
	uint8_t l_s_1; // left_scroll_1
	uint8_t l_s_2; // left_scroll_2
	uint8_t l_s_3; // left_scroll_3
	uint8_t l_s_4; // left_scroll_4
	uint8_t r_s_1; // right_scroll_1
	uint8_t r_s_2; // right_scroll_2
	uint8_t r_s_3; // right_scroll_3
	uint8_t r_s_4; // right_scroll_4
};


const uint16_t STEERING_WHEEL_MAIN_CAN_ID = 0;
const uint8_t STEERING_WHEEL_MAIN_CAN_DLC = sizeof(Steering_Wheel_main);
const uint16_t STEERING_WHEEL_NAME_CAN_ID = 0;
const uint8_t STEERING_WHEEL_NAME_CAN_DLC = sizeof(Steering_Wheel_Name);

#endif

