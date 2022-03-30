#include "guihandler.h"

using json = nlohmann::json;

GUIHandler::GUIHandler(): mainWindow(), asyncCanData(canHandler.getCanData()),
    updateTimer(new QTimer())
{
    canHandler.connect();

    QObject::connect(updateTimer, &QTimer::timeout, this, &GUIHandler::startAsync);
    updateTimer->start(1000 / frequency);
    mainWindow.show();
}

GUIHandler::~GUIHandler()
{
    delete updateTimer;
}

void GUIHandler::updateGUI()
{
    asyncCanData.mtx.lock();
    canData = static_cast<CanData>(asyncCanData);    //casting to parent because mutexes can't be copied
    asyncCanData.mtx.unlock();

    checkErrors();
    getUpdates();

}

void GUIHandler::checkErrors()
{
    for (auto const device: canData.frameArray) {
        const uint8_t  * const state = reinterpret_cast<uint8_t *>(device->dataPtr) +
                device->dlc - sizeof(uint8_t);
        if (*state not_eq 0)
            mainWindow.raiseError(QString::fromStdString(device->name) + " error");
    }
}

void GUIHandler::getUpdates()
{
    //TS
    mainWindow.updateData(Parameter::Speed, canData.ts_main.data.vehicle_speed);
    mainWindow.updateData(Parameter::CoolantTemp, canData.ts_main.data.vehicle_speed);
    //BMS LV
    mainWindow.updateData(Parameter::SOC, canData.bms_lv_main.data.soc);
    mainWindow.updateData(Parameter::BmslvTemp, canData.bms_lv_main.data.temp_avg);
    mainWindow.updateData(Parameter::BmslvVoltage, canData.bms_lv_main.data.voltage_sum);

    //...
}

void GUIHandler::getNavigation()
{
    return;
}

void GUIHandler::generateJSON()
{
//todo
    return;
}

void GUIHandler::startAsync()
{
    //todo

//    QThread thread(&GUIHandler::updateGUI);
//    th.detach();
}
