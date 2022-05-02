#pragma once

#include "vehicle.h"
#include "canhandler.h"
#include "lib/json.hpp"
#include <QtNetwork>
#include <QThread>

#include <chrono>
#include <optional>
#include <algorithm>

#define DATA_TIMEOUT_CHECK false

extern CanHandler canHandler;

class GUIHandler: public QObject
{
    Q_OBJECT
public:
    GUIHandler();
    ~GUIHandler();

signals:
    void updateData(Parameter param, qreal value);
    void error(QString const &message);
    void navigate(buttonStates navigation);
    void setPreset(Side side, scrollStates state);
    void clearError();
    void lapPassed(uint8_t sector);

private slots:
    void socketError(QAbstractSocket::SocketError const &error) const;

private:
    void updateGUI();
    void verifyData();
    void checkErrors();
    void getUpdates();
    void handleAsyncFrames();
    void connectTcpSocket();
    void generateJSON();

    QTimer * retryTimer;
    static constexpr auto retryTime = 500;

    QTcpSocket * tcpSocket;
    QString const hostname = "127.0.0.1";
    static constexpr int portNumber = 1234;

    static constexpr uint8_t relativeTelemetryFrequency{20};


    AsyncCanData const &asyncCanData;
    CanData canData;

    std::array<uint8_t, numberOfFrames> cyclesMissed;
    std::vector<DeviceBase *> errors;

    QTimer * updateTimer;
    static constexpr int frequency = 30;

    std::optional<scrollStates> scrolls[2];
    void steeringWheel();

};

