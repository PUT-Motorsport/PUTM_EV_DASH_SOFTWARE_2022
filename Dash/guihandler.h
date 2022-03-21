#pragma once

#include "vehicle.h"
#include "canhandler.h"
#include "mainwindow.h"
#include <QTimer>
#include <QObject>

class GUIHandler: public QObject
{
    Q_OBJECT
public:
    GUIHandler();
    ~GUIHandler();
private slots:
    void update();

private:
    void handleError();

    MainWindow mainWindow;
    CanData const &canData;
    QTimer * updateTimer;
    static constexpr int frequency = 30;
};

