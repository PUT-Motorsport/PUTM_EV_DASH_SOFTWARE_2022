#pragma once

#include <QThread>
#include <QtNetwork>
#include <algorithm>
#include <chrono>
#include <optional>

#include "canhandler.h"
#include "lib/json.hpp"
#include "vehicle.h"

extern CanHandler canHandler;

class GUIHandler : public QObject {
  Q_OBJECT
 public:
  GUIHandler();
  ~GUIHandler();

 signals:
  void updateData(Parameter param, float value);
  void error(QString const &message);
  void navigate(buttonStates navigation);
  void setPreset(Side side, scrollStates state);
  void clearError();
  void lapPassed(uint8_t sector);
  void setBMSHVState(BMS_HV_states state);

 private:
  void updateGUI();
  void verifyData();
  void checkErrors();
  void getUpdates();
  void handleAsyncFrames();
  void connectTcpSocket();
  void generateJSON();
  void tryUpdateData(Parameter param, float value);

  QTimer *retryTimer;
  static constexpr auto retryTime = 500;

  AsyncCanData const &asyncCanData;
  CanData canData;
  std::array<int32_t, 13> previousValues;

  std::array<uint8_t, numberOfFrames> cyclesMissed;
  std::vector<DeviceBase *> errors;

  QTimer *heartbeatTimer;

  QTimer *updateTimer;
  static constexpr int frequency = 10;

  std::optional<scrollStates> scrolls[2];
  void steeringWheel();

  BMS_HV_states previousHvState{};
};
