#include "guihandler.h"

GUIHandler::GUIHandler()
    : retryTimer(new QTimer()),
      asyncCanData(canHandler.getCanData()),
      previousValues({}),
      cyclesMissed({}),
      heartbeatTimer(new QTimer()),
      updateTimer(new QTimer()) {
  canHandler.connect();

  QObject::connect(updateTimer, &QTimer::timeout, this, &GUIHandler::updateGUI);
  updateTimer->start(1000 / frequency);

  // get information about current steering wheel scroll position

  QObject::connect(heartbeatTimer, &QTimer::timeout, &canHandler,
                   &CanHandler::heartbeat);
  heartbeatTimer->start(1000 / 10);
}

GUIHandler::~GUIHandler() {
  delete updateTimer;
  delete retryTimer;
}

void GUIHandler::updateGUI() {
  asyncCanData.mtx.lock();
  canData = static_cast<CanData>(
      asyncCanData);  // casting to parent because mutexes can't be copied
  asyncCanData.mtx.unlock();

  verifyData();
  checkErrors();
  getUpdates();
  handleAsyncFrames();

  canHandler.startNewDataCycle();
}

void GUIHandler::verifyData() {
#ifdef DATA_TIMEOUT_CHECK
  qDebug() << "Verified devices";
  for (std::size_t iter = 0; iter < numberOfFrames; iter++) {
    if (not asyncCanData.synchronousFrames.at(iter)->hasBeenUpdated) {
      cyclesMissed.at(iter)++;
    }

    if (cyclesMissed.at(iter) >
        asyncCanData.synchronousFrames.at(iter)->maxNumberOfCyclesMissed) {
      logger.add(QString::fromStdString(
                     "Device " + asyncCanData.synchronousFrames.at(iter)->name +
                     " has timed out"),
                 LogType::Error);
    }
  }

#endif
}

void GUIHandler::checkErrors() {
  // check for new errors
  for (auto const device : asyncCanData.statusFrames) {
    if (device->hasBeenUpdated) {
      uint8_t const *const state =
          reinterpret_cast<uint8_t *>(device->dataPtr) + device->dlc -
          sizeof(uint8_t);

      if (*state not_eq 0 and
          std::find(errors.begin(), errors.end(), device) ==
              errors.end()) {  // if error is registered for the first time
        emit error(QString::fromStdString(device->name) + " error");
        errors.emplace_back(const_cast<DeviceBase *>(
            device));  // cannot store const objects in vector
      }
    }
  }
  // check if old errors have been cleared

  for (auto iter = errors.begin(); iter not_eq errors.end(); ++iter) {
    if ((*iter)->hasBeenUpdated) {
      uint8_t const *const state =
          reinterpret_cast<uint8_t *>((*iter)->dataPtr) + (*iter)->dlc -
          sizeof(uint8_t);
      if (*state == 0) {
        errors.erase(iter);
        iter--;  // fix iterator degeneration
        emit clearError();
      }
    }
  }
}

void GUIHandler::getUpdates() {
  // APPS
  if (canData.apps.hasBeenUpdated) {
    tryUpdateData(Parameter::Apps, canData.apps.data.pedal_position / 5);
  }
  // TS main
  if (canData.tc_main.hasBeenUpdated) {
    tryUpdateData(Parameter::Speed, canData.tc_main.data.vehicle_speed / 100);
    tryUpdateData(Parameter::Power, canData.tc_main.data.motor_current *
                                        canData.bms_hv_main.data.voltage_sum /
                                        100);
    tryUpdateData(Parameter::RPM,
                  60 / (2 * 3.1415 / (canData.tc_main.data.engine_speed)));
    // RPM = 60 s / T_obr, T_obr = 2pi / omega
  }
  // TC temperatures
  if (canData.tc_temperatures.hasBeenUpdated) {
    tryUpdateData(Parameter::CoolantTemp,
                  canData.tc_temperatures.data.water_temp_out);
    tryUpdateData(Parameter::EngineTemp, canData.tc_temperatures.data.engine);
    tryUpdateData(Parameter::InverterTemp,
                  canData.tc_temperatures.data.inverter);
  }
  // BMS LV
  if (canData.bms_lv_main.hasBeenUpdated) {
    tryUpdateData(Parameter::LVSOC, canData.bms_lv_main.data.soc / 100);
    tryUpdateData(Parameter::BmslvTemp, canData.bms_lv_main.data.temp_avg);
    tryUpdateData(Parameter::BmslvVoltage,
                  canData.bms_lv_main.data.voltage_sum);
  }
  // BMS HV
  if (canData.bms_hv_main.hasBeenUpdated) {
    tryUpdateData(Parameter::HVSOC, canData.bms_hv_main.data.soc);
    tryUpdateData(Parameter::BmshvTemp, canData.bms_hv_main.data.temp_avg);
    tryUpdateData(Parameter::BmshvVoltage,
                  canData.bms_hv_main.data.voltage_sum);
  }
  if (canData.bms_hv_main.data.device_state not_eq previousHvState) {
    previousHvState = canData.bms_hv_main.data.device_state;
    emit setBMSHVState(previousHvState);
  }
}

void GUIHandler::handleAsyncFrames() {
  if (asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated) {
    steeringWheel();
    asyncCanData.mtx.lock();
    asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated = false;
    asyncCanData.mtx.unlock();
  }
  if (asyncCanData.asynchronousFrames.at(1)->hasBeenUpdated) {
    asyncCanData.mtx.lock();
    tryUpdateData(Parameter::Laptime, reinterpret_cast<PUTM_CAN::Lap_timer_Acc_time *>(asyncCanData.asynchronousFrames.at(1)->dataPtr)->Acc_Time);
    asyncCanData.asynchronousFrames.at(1)->hasBeenUpdated = false;
    asyncCanData.mtx.unlock();
  }
}

void GUIHandler::tryUpdateData(Parameter param, float value) {
  // check whether the gui needs to be called
  if (value not_eq previousValues.at(static_cast<uint8_t>(param))) {
    emit updateData(param, value);
    previousValues.at(static_cast<uint8_t>(param)) = value;
  }
}

void GUIHandler::steeringWheel() {
  if (canData.steering_wheel_event.data.button not_eq
      buttonStates::not_pressed) {
    emit navigate(canData.steering_wheel_event.data.button);
  }

  auto left_scroll = canData.steering_wheel_event.data.l_s_1;
  auto right_scroll = canData.steering_wheel_event.data.r_s_1;

  // the value of std::optionals should be initialized after receiving the first
  // steering wheel frame

  // left scroll
  if (not((scrolls[Side::Left]).has_value())) {
    scrolls[Side::Left].emplace(left_scroll);

  } else if (left_scroll not_eq scrolls[Side::Left]) {
    scrolls[Side::Left] = left_scroll;
  }

  // right scroll
  if (not((scrolls[Side::Right]).has_value())) {
    scrolls[Side::Right].emplace(canData.steering_wheel_event.data.r_s_1);

  } else if (left_scroll not_eq scrolls[Side::Right]) {
    scrolls[Side::Right] = right_scroll;
    emit setPreset(Side::Right, right_scroll);
  }
}
