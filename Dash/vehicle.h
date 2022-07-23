#ifndef VEHICLE_H
#define VEHICLE_H

#include <QMutex>
#include <array>
#include <queue>
#include <string>

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

template <typename T>
class Device : public DeviceBase {
 public:
  T data;
  explicit Device(uint8_t id, std::string const &name, uint8_t frequency = 0)
      : DeviceBase(id, sizeof(T), name, frequency) {
    dataPtr = &data;  // for memory copying without v-calls
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
  Device<Lap_timer_Main> laptimer_main{LAP_TIMER_MAIN_CAN_ID, "Laptimer",
                                       LAP_TIMER_MAIN_FREQUENCY};
  Device<Lap_timer_Pass> laptimer_pass{LAP_TIMER_PASS_CAN_ID, "Laptimer_pass"};
  Device<SF_main> sf_main{SF_MAIN_CAN_ID, "SF main", SF_MAIN_FREQUENCY};
  Device<SF_PassiveElements> sf_passive{SF_PASSIVEELEMENTS_CAN_ID, "sf passive",
                                        SF_PASSIVEELEMENTS_FREQUENCY};
  Device<SF_LegendaryDVAndSupply> sf_dv{SF_LEGENDARYDVANDSUPPLY_CAN_ID,
                                        "sf dv and supply",
                                        SF_LEGENDARYDVANDSUPPLY_FREQUENCY};
  Device<SF_Supply> sf_supply{SF_SUPPLY_CAN_ID, "sf supply",
                              SF_SUPPLY_FREQUENCY};
  Device<SF_safety> sf_safety{SF_SAFETY_CAN_ID, "sf safety",
                              SF_SAFETY_FREQUENCY};
  Device<Steering_Wheel_main> steering_wheel_main{
      STEERING_WHEEL_MAIN_CAN_ID, "Steering Wheel",
      STEERING_WHEEL_MAIN_FREQUENCY};
  Device<Steering_Wheel_event> steering_wheel_event{
      STEERING_WHEEL_EVENT_CAN_ID, "Steering wheel event",
      STEERING_WHEEL_EVENT_FREQUENCY};
  Device<TC_main> tc_main{TC_MAIN_CAN_ID, "TC", TC_MAIN_FREQUENCY};
  Device<TC_rear_suspension> tc_rear_suspension{
      TC_REAR_SUSPENSION_CAN_ID, "TC_rear", TC_REAR_SUSPENSION_FREQUENCY};
  Device<TC_temperatures> tc_temperatures{
      TC_TEMPERATURES_CAN_ID, "Tc temperatures", TC_TEMPERATURES_FREQUENCY};
  Device<TC_imu_gyro> tc_imu_gyro{TC_IMU_GYRO_CAN_ID, "Tc imu gyro",
                                  TC_IMU_GYRO_FREQUENCY};
  Device<TC_imu_acc> tc_imu_acc{TC_IMU_ACC_CAN_ID, "Tc imu acc",
                                TC_IMU_ACC_FREQUENCY};
  Device<Telemetry_Main> telemetry_main{TELEMETRY_MAIN_CAN_ID, "Telemetry",
                                        TELEMETRY_MAIN_FREQUENCY};
};

constexpr auto numberOfFrames = 18;

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
      &sf_passive,
      &sf_dv,
      &sf_supply,
      &sf_safety,
      &steering_wheel_main,
      &tc_main,
      &tc_rear_suspension,
      &tc_temperatures,
      &tc_imu_gyro,
      &tc_imu_acc,
      &telemetry_main};

  std::array<DeviceBase *const, 2> asynchronousFrames{&steering_wheel_event,
                                                      &laptimer_pass};

  std::array<DeviceBase const *const, 7> statusFrames{
      &apps,    &aq_main, &bms_lv_main,   &laptimer_main,
      &sf_main, &tc_main, &telemetry_main};
};

// common defs
enum class Parameter {
  Speed,
  RPM,
  HVSOC,
  LVSOC,
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
enum class Window {
  Main,
  CAN,
  DV,
  Driving,
  Logs,
  ServiceMode,
  ChangeConfirm
};  // will be used to decide GUI calls redirection
enum Side { Left, Right };
enum class Setting {
  TC,
  MaxSlipRatio,
  Algorithm,
  MaxPower,
  AppsCurve,
  RegenPower,
  Sensitivity
};
enum class Algorithm { PI, PID, LQR, LQRI, SDRE, MPC };
enum class Apps_curve { Linear, Wet, Acc };

#endif  // VEHICLE_H
