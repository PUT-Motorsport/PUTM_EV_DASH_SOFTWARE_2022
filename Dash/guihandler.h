#pragma once

#include "vehicle.h"
#include "canhandler.h"
#include "mainwindow.h"
#include "lib/json.hpp"
#include <QtNetwork>
#include <QThread>

class GUIHandler: public QObject
{
    Q_OBJECT
public:
    GUIHandler();
    ~GUIHandler();
private slots:
//    void socketError(QAbstractSocket::SocketError const &error) const;  //todo

private:
    void updateGUI();

    void verifyData();
    void checkErrors();
    void getUpdates();
    void getNavigation();

    void generateJSON() const;

    void startAsync();

    QTcpSocket * tcpSocket;
    QString const hostname = "127.0.0.1";
    static constexpr int portNumber = 1234;

    MainWindow mainWindow;
    AsyncCanData const &asyncCanData;
    CanData canData;

    std::array<uint8_t, CanData::numberOfFrames> cyclesMissed;

    QTimer * updateTimer;
    static constexpr int frequency = 30;
};

