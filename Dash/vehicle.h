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

static constexpr auto defaultMaxCyclesMissed = 5;

struct DeviceBase {
    DeviceBase(uint8_t id, uint8_t dlc, std::string const &name,
               uint8_t frequency) : id(id), dlc(dlc),
        name(name), hasBeenUpdated(true)
    {
//        if (frequency == 0)
//            maxNumberOfCyclesMissed = 0;
//        else
//            maxNumberOfCyclesMissed = 300 / frequency;
        maxNumberOfCyclesMissed = 10;
    }
    uint8_t id;
    uint8_t dlc;
    void * dataPtr;
    std::string name;
    int16_t maxNumberOfCyclesMissed;
    bool hasBeenUpdated;
    bool doesTimeout;
};

template<typename T>
class Device : public DeviceBase {
public:
    T data;
    explicit Device(uint8_t id, std::string const &name, int8_t frequency) :
        DeviceBase(id, sizeof(T), name, frequency) {
        dataPtr = &data; //for memory copying without v-calls
    }

};

struct CanData {

    static constexpr auto numberOfFrames = 11;

    Device<Apps_main> apps_main{APPS_MAIN_CAN_ID, "Apps Main", APPS_MAIN_FREQUENCY};
    Device<AQ_main> aq_main{AQ_MAIN_CAN_DLC, "AQ Main", AQ_MAIN_FREQUENCY};
    Device<BMS_LV_main> bms_lv_main{BMS_LV_MAIN_CAN_ID, "BMS LV Main", BMS_LV_MAIN_FREQUENCY};
    Device<BMS_LV_temperature> bms_lv_temperature{BMS_LV_TEMPERATURE_CAN_ID, "BMS Update Sth", BMS_LV_TEMPERATURE_FREQUENCY};
    Device<Dash_Main> dash_main{DASH_MAIN_CAN_ID, "Dash Main", DASH_MAIN_FREQUENCY};     //is it really neccessary?
    Device<Dash_StateChange> dash_state_change{DASH_STATECHANGE_CAN_ID, "Dash state change", DASH_STATECHANGE_FREQUENCY};
    Device<Lap_timer_Pass> laptimer_main{LAP_TIMER_PASS_CAN_ID, "Laptimer main", LAP_TIMER_MAIN_FREQUENCY};
    Device<SF_main> sf_main{SF_MAIN_CAN_ID, "SF main", SF_MAIN_FREQUENCY};
    Device<TS_main> ts_main{TS_MAIN_CAN_ID, "TS main", TS_MAIN_FREQUENCY};
    Device<TS_rear_suspension> ts_rear{TS_REAR_SUSPENSION_CAN_ID, "TS rear", TS_REAR_SUSPENSION_FREQUENCY};
    Device<Telemetry_Main> telemetry_main{TELEMETRY_MAIN_CAN_ID, "Telemetry main", TELEMETRY_MAIN_FREQUENCY};

    std::array<DeviceBase *, numberOfFrames> frameArray {&apps_main, &aq_main, &bms_lv_main, //all frames
                &bms_lv_temperature, &dash_main, &dash_state_change, &laptimer_main, &sf_main,
                &ts_main, &ts_rear, &telemetry_main};
    std::array<const DeviceBase *, 4> hasDeviceStatus{&apps_main, &bms_lv_main, &ts_main, &laptimer_main}; //for checking statuses

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
