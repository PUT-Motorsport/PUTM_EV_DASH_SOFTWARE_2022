#ifndef VEHICLE_H
#define VEHICLE_H

#include <QMutex>
#include <array>
#include <string>
#include <queue>
#include "CanHeaders/PM08-CANBUS-APPS.hpp"
#include "CanHeaders/PM08-CANBUS-AQ_CARD.hpp"
#include "CanHeaders/PM08-CANBUS-BMS_LV.hpp"
#include "CanHeaders/PM08-CANBUS-BMS_HV.hpp"
#include "CanHeaders/PM08-CANBUS-DASH.hpp"
#include "CanHeaders/PM08-CANBUS-LAP_TIMER.hpp"
#include "CanHeaders/PM08-CANBUS-SF.hpp"
#include "CanHeaders/PM08-CANBUS-STEERING_WHEEL.hpp"
#include "CanHeaders/PM08-CANBUS-TC.hpp"
#include "CanHeaders/PM08-CANBUS-TELEMETRY.hpp"

static constexpr auto defaultMaxCyclesMissed = 5;

struct DeviceBase {
    DeviceBase(uint8_t id, uint8_t dlc, std::string const &name,
               uint8_t frequency) : id(id), dlc(dlc),
        name(name), hasBeenUpdated(true)
    {
        if (frequency == 0)
            maxNumberOfCyclesMissed = 0;
        else
            maxNumberOfCyclesMissed = 300 / frequency;
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
    explicit Device(uint8_t id, std::string const &name, uint8_t frequency) :
        DeviceBase(id, sizeof(T), name, frequency) {
        dataPtr = &data; //for memory copying without v-calls
    }

};

struct CanData{

    static constexpr auto numberOfFrames = 20;

    Device<Apps_main> apps{ APPS_MAIN_CAN_ID, "Apps", APPS_MAIN_FREQUENCY };
    Device<AQ_main> aq_main{ AQ_MAIN_CAN_ID, "Aq main", AQ_MAIN_FREQUENCY };
    Device<AQ_air_flow> aq_air_flow{ AQ_AIR_FLOW_CAN_ID, "Aq air flow", AQ_AIR_FLOW_FREQUENCY };
    Device<BMS_HV_main> bms_hv_main{ BMS_HV_MAIN_CAN_ID, "BMS HV", BMS_HV_MAIN_FREQUENCY };
    Device<BMS_LV_main> bms_lv_main{ BMS_LV_MAIN_CAN_ID, "BMS LV main", BMS_LV_MAIN_FREQUENCY };
    Device<BMS_LV_temperature> bms_lv_temperature{ BMS_LV_TEMPERATURE_CAN_ID, "BMS LV temperatures", BMS_LV_TEMPERATURE_FREQUENCY };
    Device<Dash_Main> dash_main{ DASH_MAIN_CAN_ID, "Dash Main", DASH_MAIN_FREQUENCY };
    Device<Dash_StateChange> dash_statechange{ DASH_STATECHANGE_CAN_ID, "Dash StateChange", DASH_STATECHANGE_FREQUENCY };
    Device<Lap_timer_Main> laptimer_main{ LAP_TIMER_MAIN_CAN_ID, "Laptimer", LAP_TIMER_STATECHANGE_FREQUENCY };
    Device<Lap_timer_StateChange> laptimer_statechange{ LAP_TIMER_STATECHANGE_CAN_ID, "Laptimer StateChange", LAP_TIMER_STATECHANGE_FREQUENCY };
    Device<SF_main> sf_main{ SF_MAIN_CAN_ID, "SF main", SF_MAIN_FREQUENCY };
    Device<SF_data_frame_0> sf_data_0{ SF_DATA_FRAME_0_CAN_ID, "Sf data 0", SF_DATA_FRAME_0_FREQUENCY };
    Device<SF_data_frame_1> sf_data_1{ SF_DATA_FRAME_1_CAN_ID, "Sf data 1", SF_DATA_FRAME_1_FREQUENCY };
    Device<SF_data_frame_2> sf_data_2{ SF_DATA_FRAME_2_CAN_ID, "Sf data 2", SF_DATA_FRAME_2_FREQUENCY };
    Device<SF_data_frame_3> sf_data_3{ SF_DATA_FRAME_3_CAN_ID, "Sf data 3", SF_DATA_FRAME_3_FREQUENCY };
    Device<SF_data_frame_4> sf_data_4{ SF_DATA_FRAME_4_CAN_ID, "Sf data 4", SF_DATA_FRAME_4_FREQUENCY };
    Device<SF_data_frame_5> sf_data_5{ SF_DATA_FRAME_5_CAN_ID, "Sf data 5", SF_DATA_FRAME_5_FREQUENCY };
    Device<Steering_Wheel_main> steering_wheel_main{ STEERING_WHEEL_MAIN_CAN_ID, "Steering Wheel", STEERING_WHEEL_MAIN_FREQUENCY };
    Device<Steering_Wheel_event> steering_wheel_event{ STEERING_WHEEL_EVENT_CAN_ID, "Steering wheel event", STEERING_WHEEL_EVENT_FREQUENCY };
    Device<TS_main> ts_main{ TS_MAIN_CAN_ID, "TS", TS_MAIN_FREQUENCY };
    Device<TS_rear_suspension> ts_rear_suspension{ TS_REAR_SUSPENSION_CAN_ID , "TS_rear", TS_REAR_SUSPENSION_FREQUENCY};
    Device<Telemetry_Main> telemetry_main{ TELEMETRY_MAIN_CAN_ID, "Telemetry", TELEMETRY_MAIN_FREQUENCY };


    std::array<DeviceBase *, numberOfFrames> synchronousFrames = { &apps, &aq_main, &aq_air_flow, &bms_hv_main,
                                        &bms_lv_main, &bms_lv_temperature, &dash_main,
                                        &laptimer_main, &sf_main, &sf_data_0, &sf_data_1,
                                        &sf_data_2, &sf_data_2, &sf_data_3, &sf_data_4, &sf_data_5, &steering_wheel_main,
                                        &ts_main, &ts_rear_suspension, &telemetry_main };   //todo: not every synchronous frame has a status field

    std::array<const DeviceBase *, 4> asynchronousFrames{&dash_statechange/*?*/,  &laptimer_statechange, &steering_wheel_event};

};

struct AsyncCanData: public CanData {
    mutable QMutex mtx;
    std::queue<const DeviceBase *> queue;
};

//deprecated?
enum class Parameter{Speed, RPM, SOC, Power, BmshvVoltage, BmslvVoltage,
              InverterTemp, EngineTemp, CoolantTemp, BmshvTemp, BmslvTemp, Apps};
enum class Status{Working, InError, Unresolved};    //probably not needed
enum class Window{Main, CAN, DV, Driving, Logs, ServiceMode, ChangeConfirm};    //will be used to decide GUI calls redirection
enum class FrameType{error, update, navigation, confirmation};

#endif // VEHICLE_H
