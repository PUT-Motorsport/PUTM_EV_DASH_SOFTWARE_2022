//Generated on Thu Apr  7 17:02:52 2022
#ifndef STEERING_WHEEL
#define STEERING_WHEEL

#include <cstdint>

enum struct buttonStates: uint8_t {
	button1,	//button1
	button2,	//button2
	button3,	//button3
	button4,	//button4
	button1_2,	//button1 + button2
	button1_3,	//button1 + button3
	button1_4,	//button1 + button4
	button2_3,	//button2 + button3
	button2_4,	//button2 + button4
	button3_4,	//button3 + button4
};

enum struct scrollStates: uint8_t {
	scroll_1,
	scroll_2,
	scroll_3,
	scroll_4,	//
};


enum struct Steering_Wheel_states: uint8_t {
	OK,
	Power_up,
	Normal_operation,
};

struct __attribute__ ((packed)) Steering_Wheel_main{
	int16_t s_w_a; // "steering wheel angle [-180
	Steering_Wheel_states device_state; // -----------------------------------------------------------------
};

struct __attribute__ ((packed)) Steering_Wheel_event{
	buttonStates button; // see button state
	scrollStates l_s_1; // see button state
	scrollStates r_s_1; // see button state
};


const uint16_t STEERING_WHEEL_MAIN_CAN_ID = 0;
const uint8_t STEERING_WHEEL_MAIN_CAN_DLC = sizeof(Steering_Wheel_main);
const uint8_t STEERING_WHEEL_MAIN_FREQUENCY = 100;
const uint16_t STEERING_WHEEL_EVENT_CAN_ID = 0;
const uint8_t STEERING_WHEEL_EVENT_CAN_DLC = sizeof(Steering_Wheel_event);
const uint8_t STEERING_WHEEL_EVENT_FREQUENCY = 0;

#endif

