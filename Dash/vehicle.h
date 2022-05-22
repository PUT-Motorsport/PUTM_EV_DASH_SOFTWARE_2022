#ifndef VEHICLE_H
#define VEHICLE_H

#include "CanHeaders/PM08-CANBUS-APPS.hpp"
#include "CanHeaders/PM08-CANBUS-AQ_CARD.hpp"
#include "CanHeaders/PM08-CANBUS-BMS_HV.hpp"
#include "CanHeaders/PM08-CANBUS-BMS_LV.hpp"
#include "CanHeaders/PM08-CANBUS-DASH.hpp"
#include "CanHeaders/PM08-CANBUS-LAP_TIMER.hpp"
#include "CanHeaders/PM08-CANBUS-SF.hpp"
#include "CanHeaders/PM08-CANBUS-STEERING_WHEEL.hpp"
#include "CanHeaders/PM08-CANBUS-TC.hpp"
#include "CanHeaders/PM08-CANBUS-TELEMETRY.hpp"
#include <QMutex>
#include <array>
#include <queue>
#include <string>

struct DeviceBase {
  DeviceBase(uint8_t id, uint8_t dlc, std::string const &name,
             uint8_t frequency)
      : id(id), dlc(dlc), name(name), hasBeenUpdated(false) {
    if (frequency == 0)
      maxNumberOfCyclesMissed = 0;
    else
      maxNumberOfCyclesMissed = 300 / frequency;
  }
  uint8_t id;
  uint8_t dlc;
  void *dataPtr;
  std::string name;
  int16_t maxNumberOfCyclesMissed;
  bool hasBeenUpdated;
  bool doesTimeout;
};

template <typename T> class Device : public DeviceBase {
public:
  T data;
  explicit Device(uint8_t id, std::string const &name, uint8_t frequency = 0)
      : DeviceBase(id, sizeof(T), name, frequency) {
    dataPtr = &data; // for memory copying without v-calls
  }
};

struct CanData {

  Device<Apps_main> apps{APPS_MAIN_CAN_ID, "Apps", APPS_MAIN_FREQUENCY};
  Device<AQ_main> aq_main{AQ_MAIN_CAN_ID, "Aq main", AQ_MAIN_FREQUENCY};
  Device<BMS_HV_main> bms_hv_main{BMS_HV_MAIN_CAN_ID, "BMS HV",
                                  BMS_HV_MAIN_FREQUENCY};
  Device<BMS_LV_main> bms_lv_main{BMS_LV_MAIN_CAN_ID, "BMS LV main",
                                  BMS_LV_MAIN_FREQUENCY};
  Device<BMS_LV_temperature> bms_lv_temperature{BMS_LV_TEMPERATURE_CAN_ID,
                                                "BMS LV temperatures",
                                                BMS_LV_TEMPERATURE_FREQUENCY};
  //    Device<Dash_Main> dash_main{ DASH_MAIN_CAN_ID, "Dash Main",
  //    DASH_MAIN_FREQUENCY }; Device<Dash_StateChange> dash_statechange{
  //    DASH_STATECHANGE_CAN_ID, "Dash StateChange", DASH_STATECHANGE_FREQUENCY
  //    };
  Device<Lap_timer_Main> laptimer_main{LAP_TIMER_MAIN_CAN_ID, "Laptimer",
                                       LAP_TIMER_MAIN_FREQUENCY};
  Device<Lap_timer_Pass> laptimer_pass{LAP_TIMER_PASS_CAN_ID, "Laptimer_pass"};
  Device<SF_main> sf_main{SF_MAIN_CAN_ID, "SF main", SF_MAIN_FREQUENCY};
  Device<SF_FrontBox> sf_frontBox{SF_FRONTBOX_CAN_ID, "sf front box",
                                  SF_FRONTBOX_FREQUENCY};
  Device<SF_CoolingAndVSafety> sf_cooling{SF_COOLINGANDVSAFETY_CAN_ID,
                                          "sf cooling",
                                          SF_COOLINGANDVSAFETY_FREQUENCY};
  Device<SF_DV> sf_dv{SF_DV_CAN_ID, "sf dv", SF_DV_FREQUENCY};
  Device<SF_WS> sf_ws{SF_WS_CAN_ID, "sf ws", SF_WS_FREQUENCY};
  Device<SF_NUCS> sf_nucs{SF_NUCS_CAN_ID, "sf nucs", SF_NUCS_FREQUENCY};
  Device<Steering_Wheel_main> steering_wheel_main{
      STEERING_WHEEL_MAIN_CAN_ID, "Steering Wheel",
      STEERING_WHEEL_MAIN_FREQUENCY};
  Device<Steering_Wheel_event> steering_wheel_event{
      STEERING_WHEEL_EVENT_CAN_ID, "Steering wheel event",
      STEERING_WHEEL_EVENT_FREQUENCY};
  Device<TS_main> ts_main{TS_MAIN_CAN_ID, "TS", TS_MAIN_FREQUENCY};
  Device<TS_rear_suspension> ts_rear_suspension{
      TS_REAR_SUSPENSION_CAN_ID, "TS_rear", TS_REAR_SUSPENSION_FREQUENCY};
  Device<Telemetry_Main> telemetry_main{TELEMETRY_MAIN_CAN_ID, "Telemetry",
                                        TELEMETRY_MAIN_FREQUENCY};
};

constexpr auto numberOfFrames = 15;

struct AsyncCanData : public CanData {
  mutable QMutex mtx;

  std::array<DeviceBase *const, numberOfFrames> synchronousFrames = {
      &apps,
      &aq_main,
      &bms_hv_main,
      &bms_lv_main,
      &bms_lv_temperature,
      &laptimer_main,
      &sf_main,
      &sf_cooling,
      &sf_dv,
      &sf_ws,
      &sf_nucs,
      &steering_wheel_main,
      &ts_main,
      &ts_rear_suspension,
      &telemetry_main};

  std::array<DeviceBase *const, 2> asynchronousFrames{&steering_wheel_event,
                                                      &laptimer_pass};

  std::array<DeviceBase const *const, 8> statusFrames{
      &apps,          &aq_main, &bms_hv_main, &bms_lv_main,
      &laptimer_main, &sf_main, &ts_main,     &telemetry_main};
};

enum class Parameter {
  Speed,
  RPM,
  SOC,
  Power,
  BmshvVoltage,
  BmslvVoltage,
  InverterTemp,
  EngineTemp,
  CoolantTemp,
  BmshvTemp,
  BmslvTemp,
  Apps
};
enum class Status { Working, InError, Unresolved }; // probably not needed
enum class Window {
  Main,
  CAN,
  DV,
  Driving,
  Logs,
  ServiceMode,
  ChangeConfirm
}; // will be used to decide GUI calls redirection
enum Side { Left, Right };

#endif // VEHICLE_H
