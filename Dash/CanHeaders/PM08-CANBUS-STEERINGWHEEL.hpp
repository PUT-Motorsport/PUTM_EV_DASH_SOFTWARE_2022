//Generated on Fri Apr  1 17:39:57 2022
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
    ScrollState1,
    ScrollState2,
    ScrollState3,
    ScrollState4
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

struct __attribute__ ((packed)) Steering_Wheel_buttonPress{
    buttonStates buttonChanged;
    scrollStates leftScroll;
    scrollStates rightScroll;
};


const uint16_t STEERING_WHEEL_MAIN_CAN_ID = 0;
const uint8_t STEERING_WHEEL_MAIN_CAN_DLC = sizeof(Steering_Wheel_main);
const uint8_t STEERING_WHEEL_MAIN_FREQUENCY = 100;
const uint16_t STEERING_WHEEL_BUTTONPRESS_CAN_ID = 0;
const uint8_t STEERING_WHEEL_BUTTONPRESS_CAN_DLC = sizeof(Steering_Wheel_buttonPress);
const uint8_t STEERING_WHEEL_BUTTONPRESS_FREQUENCY = 0;
#endif
