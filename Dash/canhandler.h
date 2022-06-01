#pragma once

#include <cstring> //memcpy

#include "logger.h"
#include "vehicle.h"
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QObject>
#include <QTimer>
#include <QThread>

extern Logger logger;

class CanHandler : public QObject {
  Q_OBJECT
public:
  CanHandler(QObject *parent = nullptr);
  ~CanHandler();

  bool connect();
  bool connected();

  bool send(QCanBusFrame const &toSend);
  void setHeartbeat(Dash_states newState) { heartBeatState = newState; }

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

  QTimer *retryTimer; // todo
  static constexpr auto retryTime = 200;

  Dash_states heartBeatState;
};
