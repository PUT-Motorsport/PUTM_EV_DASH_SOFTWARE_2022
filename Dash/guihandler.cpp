#include "guihandler.h"

GUIHandler::GUIHandler(): retryTimer(new QTimer()), tcpSocket(new QTcpSocket()),
    asyncCanData(canHandler.getCanData()), updateTimer(new QTimer())
{
    qDebug() << "GUI Handler created";
    canHandler.connect();

    QObject::connect(updateTimer, &QTimer::timeout, this, &GUIHandler::updateGUI);
    updateTimer->start(1000 / frequency);

    connectTcpSocket();
//    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), //qt issue
//    this, SLOT(socketError(QAbstractSocketError &error)));

    for (auto &item: cyclesMissed)
        item = 0;   //initialize the array

//    //testing
//    generateJSON();

    //get information about current steering wheel scroll position

    Dash_steering_wheel_request request{};

    QCanBusFrame frame{};
    frame.setFrameId(DASH_STEERING_WHEEL_REQUEST_CAN_ID);
    frame.setPayload(QByteArray(reinterpret_cast<char *>(&request), sizeof(Dash_steering_wheel_request)));

    if (not canHandler.send(frame)) {
        logger.add("Steering wheel request not sent", LogType::AppError);
    }

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

    static uint8_t cycle{0};

//    if (cycle % relativeTelemetryFrequency == 0)
//        generateJSON();

    cycle++;

    verifyData();
    checkErrors();
    getUpdates();
    handleAsyncFrames();

    canHandler.startNewDataCycle();

}

void GUIHandler::verifyData()
{

#ifdef DATA_TIMEOUT_CHECK
    qDebug() << "Verified devices";
    for (std::size_t iter = 0; iter < numberOfFrames; iter++)  {

        if (not asyncCanData.synchronousFrames.at(iter)->hasBeenUpdated) {
            cyclesMissed.at(iter)++;
        }

        if (cyclesMissed.at(iter) > asyncCanData.synchronousFrames.at(iter)->maxNumberOfCyclesMissed) {
            logger.add(QString::fromStdString("Device " + asyncCanData.synchronousFrames.at(iter)->name
                                                         + " has timed out"), LogType::Error);
        }
    }

#endif

}

void GUIHandler::checkErrors()
{
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
        emit updateData(Parameter::Power, canData.ts_main.data.vehicle_speed *
                        canData.bms_hv_main.data.voltage_sum);
        emit updateData(Parameter::RPM, canData.ts_main.data.vehicle_speed);
        emit updateData(Parameter::InverterTemp, canData.ts_main.data.motor_current);
    }
    //BMS LV
    if (canData.bms_lv_main.hasBeenUpdated == true) {
        emit updateData(Parameter::SOC, canData.bms_lv_main.data.soc);
        emit updateData(Parameter::BmslvTemp, canData.bms_lv_main.data.temp_avg);
        emit updateData(Parameter::BmslvVoltage, canData.bms_lv_main.data.voltage_sum);
    }
    //BMS HV
    if (canData.bms_hv_main.hasBeenUpdated == true) {
        emit updateData(Parameter::BmshvTemp, canData.bms_hv_main.data.temp_avg);
        emit updateData(Parameter::BmshvVoltage, canData.bms_hv_main.data.voltage_sum);

    }
}

void GUIHandler::handleAsyncFrames()
{
    if (asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated) {
        steeringWheel();
        asyncCanData.mtx.lock();
        asyncCanData.asynchronousFrames.at(0)->hasBeenUpdated = false;
        asyncCanData.mtx.unlock();
    }
    if (asyncCanData.asynchronousFrames.at(1)->hasBeenUpdated) {
        emit lapPassed(canData.laptimer_pass.data.Sector);
        asyncCanData.mtx.lock();
        asyncCanData.asynchronousFrames.at(1)->hasBeenUpdated = false;
        asyncCanData.mtx.unlock();
    }
}

void GUIHandler::generateJSON()
{
    nlohmann::json telemetry;
    //timestamp: posix time
    telemetry["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    telemetry["APPS"]   = {{"pedal_position", static_cast<uint>(canData.apps.data.pedal_position)},
                          {"position_diff", canData.apps.data.position_diff},
                          {"state"}, static_cast<int>(canData.apps.data.device_state)};
    telemetry["BMS LV"] = {{"voltage_sum", static_cast<int>(canData.bms_lv_main.data.voltage_sum)},
                          {"soc", canData.bms_lv_main.data.soc},
                          {"temp_avg", canData.bms_lv_main.data.temp_avg},
                          {"current", canData.bms_lv_main.data.current},
                          {"state", static_cast<uint>(canData.bms_lv_main.data.device_state)},
                          {"temp1", canData.bms_lv_temperature.data.temp_1},
                          {"temp2", canData.bms_lv_temperature.data.temp_2},
                          {"temp3", canData.bms_lv_temperature.data.temp_3},
                          {"temp4", canData.bms_lv_temperature.data.temp_4},
                          {"temp5", canData.bms_lv_temperature.data.temp_5},
                          {"temp6", canData.bms_lv_temperature.data.temp_6},
                          {"temp7", canData.bms_lv_temperature.data.temp_7},
                          {"temp8", canData.bms_lv_temperature.data.temp_8}};
    telemetry["BMS HV"] = {{"voltage_sum", static_cast<uint>(canData.bms_hv_main.data.voltage_sum)},
                          {"soc", canData.bms_hv_main.data.soc},
                          {"temp_max", canData.bms_hv_main.data.temp_max},
                          {"temp_avg", canData.bms_hv_main.data.temp_avg},
                          {"current", canData.bms_hv_main.data.current},
                          {"state", canData.bms_hv_main.data.device_state}};
    telemetry["Laptimer"] = {{"state", canData.laptimer_main.data.device_state}};
    telemetry["SF"]       = {{}};
    telemetry["Steering Wheel"] = {{"steering_wheel_angle", static_cast<uint>(canData.steering_wheel_main.data.s_w_a)},
                                  {"state", canData.steering_wheel_main.data.device_state}};
    telemetry["TC"]     = {{"vehicle_speed", static_cast<uint>(canData.ts_main.data.vehicle_speed)},
                          {"water_temp", canData.ts_main.data.water_temp},
                          {"water_pressure", canData.ts_main.data.water_pressure},
                          {"motor_current", canData.ts_main.data.motor_current},
                          {"tractive_system_on", static_cast<bool>(canData.ts_main.data.tractive_system_on)},
                          {"rtds_active", static_cast<bool>(canData.ts_main.data.rtds_active)},
                          {"traction_control_enable", static_cast<bool>(canData.ts_main.data.traction_control_enable)},
                          {"regen_enable", static_cast<bool>(canData.ts_main.data.regen_enable)},
                          {"traction_control_intesivity", canData.ts_main.data.traction_control_intensivity},
                          {"adc_susp_right", static_cast<uint>(canData.ts_rear_suspension.data.adc_susp_right)},
                          {"adc_susp_left", static_cast<uint>(canData.ts_rear_suspension.data.adc_susp_left)},
                          {"acc_latteral", static_cast<uint>(canData.ts_rear_suspension.data.acc_lateral)},
                          {"acc_longitunal", static_cast<uint>(canData.ts_rear_suspension.data.acc_longitunal)},
                          {"state", canData.ts_main.data.device_state}};
    telemetry["Telemetry"] = {{"state", canData.telemetry_main.data.device_state}};

    if (tcpSocket->write(reinterpret_cast<char *>(&telemetry), sizeof(telemetry))) {
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

    }

    //right scroll
    if (not((scrolls[Side::Right]).has_value())) {

        scrolls[Side::Right].emplace(canData.steering_wheel_event.data.r_s_1);

    }
    else if (left_scroll not_eq scrolls[Side::Right]) {

        scrolls[Side::Right] = right_scroll;
        emit setPreset(Side::Right, right_scroll);

    }
}
