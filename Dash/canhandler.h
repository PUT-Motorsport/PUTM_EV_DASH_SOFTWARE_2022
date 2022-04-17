#pragma once

#include <cstring> //memcpy

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QTimer>
#include "vehicle.h"
#include "logger.h"

extern Logger logger;

class CanHandler : public QObject{
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


private slots:
    void onCanFrameReceived();
    void onCanErrorOcurred();
private:
    QCanBusDevice *canDevice;
    AsyncCanData canData;
    std::queue<DeviceBase *> asyncFrames;

    QTimer * retryTimer;    //todo
    static constexpr auto retryTime = 200;

    //heartbeat
    void heartbeat();

    Dash_states heartBeatState;
    QTimer * heartbeatTimer;
    static constexpr auto hearbeatFrequency = 10;
};
