#pragma once

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>

#include <array>

#include "vehicle.h"
#include "logger.h"

extern Logger logger;

class CanHandler : public QObject{
    Q_OBJECT
public:
    CanHandler(QObject *parent = nullptr);
    ~CanHandler();

    bool connect();
    bool connected() { return m_connected; }

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

    bool m_connected;

    //heartbeat
    Dash_states heartBeatState;
};
