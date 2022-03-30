#pragma once

#include "vehicle.h"
#include "canhandler.h"
#include "mainwindow.h"
#include "lib/json.hpp"
#include <QThread>

class GUIHandler: public QObject
{
    Q_OBJECT
public:
    GUIHandler();
    ~GUIHandler();
private:
    void updateGUI();

    void checkErrors();
    void getUpdates();
    void getNavigation();

    void generateJSON();

    void startAsync();

    MainWindow mainWindow;
    AsyncCanData const &asyncCanData;
    CanData canData;
    QTimer * updateTimer;
    static constexpr int frequency = 30;
};

