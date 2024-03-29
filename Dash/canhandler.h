#pragma once

#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <cstring>  //memcpy

#include "CanHeaders/PM08-CANBUS-DASH.hpp"
#include "logger.h"
#include "vehicle.h"

extern Logger logger;

class CanHandler : public QObject {
  Q_OBJECT
 public:
  CanHandler(QObject *parent = nullptr);
  ~CanHandler();

  bool connect();
  bool connected();

  bool send(QCanBusFrame const &toSend);
  [[deprecated]] void setHeartbeat(Dash_states newState) {
    heartBeatState = newState;
  }

  AsyncCanData const &getCanData() { return canData; }
  void startNewDataCycle();

 public slots:
  void heartbeat();

 private slots:
  void onCanFrameReceived();
  void onCanErrorOcurred();

 private:
  QCanBusDevice *canDevice;
  AsyncCanData canData;

  Dash_states heartBeatState;
};
