#include "guihandler.h"

GUIHandler::GUIHandler(): mainWindow(), canData(canHandler.getCanData()),
    updateTimer(new QTimer())
{
    canHandler.connect();

    //will be used to control gui updates from another thread
    QObject::connect(updateTimer, &QTimer::timeout, this, &GUIHandler::update);
    updateTimer->start(1000 / frequency);
    mainWindow.show();
}

GUIHandler::~GUIHandler()
{
    delete updateTimer;
}

void GUIHandler::update()
{
    canData.canDataMtx.lock();
//    CanData canDataCopy = canData;  //fixme: add constructor
    canData.canDataMtx.unlock();

    if (canData.apps_main.data.device_state not_eq Apps_states::Normal_operation)
        handleError();
    if (canData.bms_lv_main.data.device_state not_eq BMS_LV_states::Normal)
        handleError();
    if (canData.ts_main.data.device_state not_eq TS_states::OK)
        handleError();
    //all devices
    mainWindow.updateData(Parameter::Speed, canData.ts_main.data.vehicle_speed);    //fixme: better way to access all data
}

void GUIHandler::handleError()
{
    return;
}
