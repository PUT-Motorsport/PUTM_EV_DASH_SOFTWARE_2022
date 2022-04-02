//Generated on Fri Apr  1 17:44:25 2022
#ifndef SF
#define SF

#include <cstdint>

typedef struct { uint16_t ok : 1;
    uint16_t overheat : 1;
    uint16_t undercurrent : 1;
    uint16_t overcurrent : 1;
    uint16_t current : 12;
}FuseData;


enum struct SF_states: uint8_t {
    OK,
    CERROR,	// critical error
    WARNING_1,
    FUSE_0_FAILED,
    FUSE_1_FAILED,
    FUSE_2_FAILED,
    FUSE_3_FAILED,
};

struct __attribute__ ((packed)) SF_main{
    FuseData fuses_overall_state; // combined states + sum of all currenst
    SF_states device_state; // -----------------------------------------------------------------
};

struct __attribute__ ((packed)) SF_main1{
    FuseData fuse_0_inverter; // state + mA read
    FuseData fuse_0_boxf_mb; // state + mA read
    FuseData fuse_0_apps; // state + mA read
    FuseData fuse_0_box_dv; // state + mA read
};

struct __attribute__ ((packed)) SF_main2{
    FuseData fuse_0_tsal_logic; // state + mA read
    FuseData fuse_0_tsal_hv; // state + mA read
    FuseData fuse_1_ws_fl; // state + mA read
    FuseData fuse_1_ws_rr; // state + mA read
};

struct __attribute__ ((packed)) SF_main3{
    FuseData fuse_1_ws_rl; // state + mA read
    FuseData fuse_1_fan_l; // state + mA read
    FuseData fuse_1_fan_r; // state + mA read
    FuseData fuse_1_dashboard; // state + mA read
};

struct __attribute__ ((packed)) SF_main4{
    FuseData fuse_2_v_safety; // state + mA read
    FuseData fuse_2_wheel; // state + mA read
    FuseData fuse_2_ws_fr; // state + mA read
    FuseData fuse_2_intel_nuc; // state + mA read
};

struct __attribute__ ((packed)) SF_main5{
    FuseData fuse_2_jetson; // state + mA read
    FuseData fuse_3_spare_1; // state + mA read
    FuseData fuse_3_spare_2; // state + mA read
    FuseData fuse_3_spare_3; // state + mA read
};

struct __attribute__ ((packed)) SF_main6{
    FuseData fuse_3_pump; // state + mA read
};

const uint16_t SF_MAIN_CAN_ID = 0;
const uint8_t SF_MAIN_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN_FREQUENCY = 100;
const uint16_t SF_MAIN1_CAN_ID = 0;
const uint8_t SF_MAIN1_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN1_FREQUENCY = 10;
const uint16_t SF_MAIN2_CAN_ID = 0;
const uint8_t SF_MAIN2_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN2_FREQUENCY = 10;
const uint16_t SF_MAIN3_CAN_ID = 0;
const uint8_t SF_MAIN3_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN3_FREQUENCY = 10;
const uint16_t SF_MAIN4_CAN_ID = 0;
const uint8_t SF_MAIN4_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN4_FREQUENCY = 10;
const uint16_t SF_MAIN5_CAN_ID = 0;
const uint8_t SF_MAIN5_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN5_FREQUENCY = 10;
const uint16_t SF_MAIN6_CAN_ID = 0;
const uint8_t SF_MAIN6_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN6_FREQUENCY = 10;

#endif
