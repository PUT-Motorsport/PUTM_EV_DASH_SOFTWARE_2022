//Generated on Mon Mar 28 16:33:14 2022
#ifndef SF
#define SF

#include <cstdint>

typedef struct 
{   uint16_t ok : 1;
    uint16_t overheat : 1;
    uint16_t undercurrent : 1;
    uint16_t overcurrent : 1;
    uint16_t current : 12;
} FuseData;

enum struct SF_states: uint8_t {
	OK,
	CERROR,
	FUSE_0_FAILED,
	FUSE_1_FAILED,
	FUSE_2_FAILED,	// std::string strcopy = target;
	FUSE_3_FAILED,
};

struct __attribute__ ((packed)) SF_main{
	FuseData fuse_0_inverter; // state + mA read
	FuseData fuse_0_boxf_mb; // state + mA read
	FuseData fuse_0_apps; // state + mA read
	FuseData fuse_0_box_dv; // state + mA read
	FuseData fuse_0_tsal_logic; // state + mA read
	FuseData fuse_0_tsal_hv; // state + mA read
	FuseData fuse_1_ws_fl; // state + mA read
	FuseData fuse_1_ws_rr; // state + mA read
	FuseData fuse_1_ws_rl; // state + mA read
	FuseData fuse_1_fan_l; // state + mA read
	FuseData fuse_1_fan_r; // state + mA read
	FuseData fuse_1_dashboard; // state + mA read
	FuseData fuse_2_v_safety; // state + mA read
	FuseData fuse_2_wheel; // state + mA read
	FuseData fuse_2_ws_fr; // state + mA read
	FuseData fuse_2_intel_nuc; // state + mA read
	FuseData fuse_2_jetson; // state + mA read
	FuseData fuse_3_spare_1; // state + mA read
	FuseData fuse_3_spare_2; // state + mA read
	FuseData fuse_3_spare_3; // state + mA read
	FuseData fuse_3_pump; // state + mA read
	SF_states device_state; // -----------------------------------------------------------------
};

const uint16_t SF_MAIN_CAN_ID = 0;
const uint8_t SF_MAIN_CAN_DLC = sizeof(SF_main);

#endif

