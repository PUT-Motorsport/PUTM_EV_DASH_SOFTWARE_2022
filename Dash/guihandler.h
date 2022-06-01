#pragma once

#include "canhandler.h"
#include "lib/json.hpp"
#include "vehicle.h"
#include <QThread>
#include <QtNetwork>

#include <algorithm>
#include <chrono>
#include <optional>

extern CanHandler canHandler;

class GUIHandler : public QObject {
  Q_OBJECT
public:
  GUIHandler();
  ~GUIHandler();

signals:
  void updateData(Parameter param, int32_t value);
  void error(QString const &message);
  void navigate(buttonStates navigation);
  void setPreset(Side side, scrollStates state);
  void clearError();
  void lapPassed(uint8_t sector);

private:
  void updateGUI();
  void verifyData();
  void checkErrors();
  void getUpdates();
  void handleAsyncFrames();
  void connectTcpSocket();
  void generateJSON();
  void tryUpdateData(Parameter param, int value);

  QTimer *retryTimer;
  static constexpr auto retryTime = 500;

  AsyncCanData const &asyncCanData;
  CanData canData;
  std::array<int32_t, 12> previousValues;

  std::array<uint8_t, numberOfFrames> cyclesMissed;
  std::vector<DeviceBase *> errors;

  QTimer * heartbeatTimer;

  QTimer *updateTimer;
  static constexpr int frequency = 5;

  std::optional<scrollStates> scrolls[2];
  void steeringWheel();
};
