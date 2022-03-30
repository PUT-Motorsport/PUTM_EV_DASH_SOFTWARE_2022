#ifndef VEHICLE_H
#define VEHICLE_H

#include <QMutex>
#include <array>
#include <string>
#include "CanHeaders/PM08-CANBUS-APPS.hpp"
#include "CanHeaders/PM08-CANBUS-AQCARD.hpp"
#include "CanHeaders/PM08-CANBUS-BMSLV.hpp"
#include "CanHeaders/PM08-CANBUS-DASH.hpp"
#include "CanHeaders/PM08-CANBUS-LAPTIMER.hpp"
#include "CanHeaders/PM08-CANBUS-SF.hpp"
#include "CanHeaders/PM08-CANBUS-TC.hpp"
#include "CanHeaders/PM08-CANBUS-TELEMETRY.hpp"

struct DeviceBase {
    DeviceBase(uint8_t id, uint8_t dlc) : id(id), dlc(dlc) {}
    uint8_t id;
    uint8_t dlc;
    void * dataPtr;
    std::string name;   //todo: add initialization
};
template<typename T>
class Device : public DeviceBase {
public:
    T data;
    explicit Device(uint8_t id) : DeviceBase(id, sizeof(T)) {
        dataPtr = &data; //for memory copying without v-calls
    }

};

struct CanData {

    Device<Apps_main> apps_main{APPS_MAIN_CAN_ID};
    Device<AQ_main> aq_main{AQ_MAIN_CAN_DLC};
    Device<BMS_LV_main> bms_lv_main{BMS_LV_MAIN_CAN_ID};
    Device<BMS_LV_update_sth> bms_lv_update_sth{BMS_LV_UPDATE_STH_CAN_ID};
    Device<Dash_Main> dash_main{DASH_MAIN_CAN_ID};
    Device<Dash_StateChange> dash_state_change{DASH_STATECHANGE_CAN_ID};
    Device<Lap_timer_Pass> laptimer_main{LAP_TIMER_PASS_CAN_ID};
    Device<SF_main> sf_main{SF_MAIN_CAN_ID};
    Device<TS_main> ts_main{TS_MAIN_CAN_ID};
    Device<TS_rear_suspension> ts_rear{TS_REAR_SUSPENSION_CAN_ID};
    Device<Telemetry_Main> telemetry_main{TELEMETRY_MAIN_CAN_ID};

    std::array<const DeviceBase *, 7> frameArray {&apps_main, &bms_lv_main, &bms_lv_update_sth, &dash_main,
                                                  &dash_state_change, &ts_main, &ts_rear};
};

struct AsyncCanData: public CanData {
    mutable QMutex mtx;
};

//deprecated?
enum class Parameter{Speed, RPM, SOC, Power, BmshvVoltage, BmslvVoltage,
              InverterTemp, EngineTemp, CoolantTemp, BmshvTemp, BmslvTemp, Apps};
enum class Status{Working, InError, Unresolved};    //probably not needed
enum class Window{Main, CAN, DV, Driving, Logs, ServiceMode, ChangeConfirm};    //will be used to decide GUI calls redirection
enum class Navigation{A, B, X, Y, LeftDial, RightDial};
enum class FrameType{error, update, navigation, confirmation};

#endif // VEHICLE_H
