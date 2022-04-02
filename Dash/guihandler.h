#pragma once

#include "vehicle.h"
#include "canhandler.h"
#include "mainwindow.h"
#include "lib/json.hpp"
#include <QtNetwork>
#include <QThread>
#include <QtConcurrent>

#define DATA_TIMEOUT_CHECK false

class GUIHandler: public QObject
{
    Q_OBJECT
public:
    GUIHandler();
    ~GUIHandler();
private slots:
    void socketError(QAbstractSocket::SocketError const &error) const;

private:
    void connectTcpSocket();
    QTimer * retryTimer;
    static constexpr auto retryTime = 500;

    QTcpSocket * tcpSocket;
    QString const hostname = "127.0.0.1";
    static constexpr int portNumber = 631;

    void updateGUI();

    void verifyData();
    void checkErrors();
    void getUpdates();
    void getNavigation();

    void generateJSON() const;

    void startAsync();

    MainWindow mainWindow;
    AsyncCanData const &asyncCanData;
    CanData canData;

    std::array<uint8_t, CanData::numberOfFrames> cyclesMissed;

    QTimer * updateTimer;
    static constexpr int frequency = 30;
};

