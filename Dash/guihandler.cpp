#include "guihandler.h"

GUIHandler::GUIHandler(): retryTimer(new QTimer()), tcpSocket(new QTcpSocket()),
    asyncCanData(canHandler.getCanData()), updateTimer(new QTimer())
{
    canHandler.connect();

    QObject::connect(updateTimer, &QTimer::timeout, this, &GUIHandler::updateGUI);
    updateTimer->start(1000 / frequency);

    connectTcpSocket();
//    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), //qt issue
//    this, SLOT(socketError(QAbstractSocketError &error)));

    for (auto &item: cyclesMissed)
        item = 0;   //initialize the array

    //testing
    generateJSON();
}

GUIHandler::~GUIHandler()
{
    delete updateTimer;
    delete retryTimer;
    delete tcpSocket;
}

void GUIHandler::socketError(const QAbstractSocket::SocketError &error) const
{
    logger.add("Connection with localhost lost. Reason: " + QString::number(error) + ". Retrying.");

}

void GUIHandler::connectTcpSocket()
{
    tcpSocket->connectToHost(hostname, portNumber); //fixme: may not establish the connection before it is checked


    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        logger.add("Socket connection established.");
        return;
    }
    logger.add("Socket connection failed.");
    retryTimer->start(retryTime);
}

void GUIHandler::updateGUI()
{
    asyncCanData.mtx.lock();
    canData = static_cast<CanData>(asyncCanData);    //casting to parent because mutexes can't be copied
    asyncCanData.mtx.unlock();

    verifyData();
    checkErrors();
    getUpdates();
    handleAsyncFrames();

    canHandler.startNewDataCycle();

}

void GUIHandler::verifyData()
{
#if DATA_TIMEOUT_CHECK
    qDebug() << "Verified devices";
    for (std::size_t iter = 0; iter < canData.hasDeviceStatus.size(); iter++) {
        if (canData.hasDeviceStatus.at(iter)->maxNumberOfCyclesMissed == 0)
            continue;   //if max number of cycles missed is -1, no data check should be performed

        if (cyclesMissed.at(iter) >= canData.hasDeviceStatus.at(iter)->maxNumberOfCyclesMissed) {
            mainWindow.raiseError(QString::fromStdString("Device " +
                                 canData.hasDeviceStatus.at(iter)->name + " missed too many cycles"));
        }
        cyclesMissed.at(iter)++;
    }
#endif

}

void GUIHandler::checkErrors()
{
//    for (auto const device: asyncCanData.synchronousFrames) {
//        const uint8_t  * const state = reinterpret_cast<uint8_t *>(device->dataPtr) + device->dlc - sizeof(uint8_t);
//        if (*state not_eq 0 and std::find(errors.begin(), errors.end(), device) == errors.end())
//            emit error(QString::fromStdString(device->name) + " error");
//    }
    //check for new errors
    for (auto const device: asyncCanData.synchronousFrames) {
        if (device->hasBeenUpdated) {

            uint8_t const * const state = reinterpret_cast<uint8_t *>(device->dataPtr) + device->dlc - sizeof(uint8_t);

            if (*state not_eq 0 and std::find(errors.begin(), errors.end(), device) == errors.end()) {  //if error is registered for the first time
                emit error(QString::fromStdString(device->name) + " error");
                errors.emplace_back(const_cast<DeviceBase *>(device));  //cannot store const objects in vector
            }
        }
    }
    //check if old errors have been cleared

    for (auto iter = errors.begin(); iter not_eq errors.end(); ++iter) {
        if ((*iter)->hasBeenUpdated) {
            uint8_t const * const state = reinterpret_cast<uint8_t *>((*iter)->dataPtr) + (*iter)->dlc - sizeof(uint8_t);
            if (*state == 0) {
                errors.erase(iter);
                iter--; //fix iterator degeneration
                emit clearError();
            }
        }
    }
//    qDebug() << errors.size();
}

void GUIHandler::getUpdates()
{
    //TS
    if (canData.ts_main.hasBeenUpdated == true) {
        emit updateData(Parameter::Speed, canData.ts_main.data.vehicle_speed);
        emit updateData(Parameter::CoolantTemp, canData.ts_main.data.water_temp);
    }
    //BMS LV
    if (canData.bms_lv_main.hasBeenUpdated == true) {
        emit updateData(Parameter::SOC, canData.bms_lv_main.data.soc);
        emit updateData(Parameter::BmslvTemp, canData.bms_lv_main.data.temp_avg);
        emit updateData(Parameter::BmslvVoltage, canData.bms_lv_main.data.voltage_sum);
    }
    //...
}

void GUIHandler::handleAsyncFrames()
{
    if (asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated) {
        steeringWheel();
        asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated = false;
    }
    if (asyncCanData.asynchronousFrames.at(1)->hasBeenUpdated)
        ; //lapTimer(); //todo: laptimer support
}

void GUIHandler::generateJSON()
{

    //timestamp: posix time
    telemetry["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
//    telemetry["APPS"] = {{"pedal_position", canData.apps.data.pedal_position},
//                         {"position_diff", canData.apps.data.position_diff},
//                         {"state"}, static_cast<int>(canData.apps.data.device_state)};
//    telemetry["BMS LV"] = {{"voltage_sum", canData.bms_lv_main.data.voltage_sum},
//                          {"soc", canData.bms_lv_main.data.soc},
//                          {"temp_avg", canData.bms_lv_main.data.temp_avg},
//                          {"current", canData.bms_lv_main.data.current},
//                          {"state", static_cast<int>(canData.bms_lv_main.data.device_state)},
//                          {"temp1", canData.bms_lv_temperature.data.temp_1},
//                          {"temp2", canData.bms_lv_temperature.data.temp_2},
//                          {"temp3", canData.bms_lv_temperature.data.temp_3},
//                          {"temp4", canData.bms_lv_temperature.data.temp_4},
//                          {"temp5", canData.bms_lv_temperature.data.temp_5},
//                          {"temp6", canData.bms_lv_temperature.data.temp_6},
//                          {"temp7", canData.bms_lv_temperature.data.temp_7},
//                          {"temp8", canData.bms_lv_temperature.data.temp_8}};


    if (tcpSocket->write(reinterpret_cast<char *>(&telemetry), sizeof(telemetry)) == -1) {
        logger.add("Data sending failed");
    }
}

void GUIHandler::steeringWheel()
{
    if (canData.steering_wheel_event.data.button not_eq buttonStates::not_pressed) {

        emit navigate(canData.steering_wheel_event.data.button);

    }

    auto left_scroll = canData.steering_wheel_event.data.l_s_1;
    auto right_scroll = canData.steering_wheel_event.data.r_s_1;

    //left scroll
    if (not((scrolls[Side::Left]).has_value())) {

        scrolls[Side::Left].emplace(left_scroll);

    }
    else if (left_scroll not_eq scrolls[Side::Left]) {

        scrolls[Side::Left] = left_scroll;
        emit getConfirmation(Side::Left, left_scroll);

    }

    //right scroll
    if (not((scrolls[Side::Right]).has_value())) {

        scrolls[Side::Right].emplace(canData.steering_wheel_event.data.r_s_1);

    }
    else if (left_scroll not_eq scrolls[Side::Right]) {

        scrolls[Side::Right] = right_scroll;
        emit getConfirmation(Side::Right, right_scroll);

    }
}
