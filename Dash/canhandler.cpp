#include "canhandler.h"

CanHandler::CanHandler(QObject *parent): QObject(parent),
    heartbeatTimer(new QTimer())
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        logger.add("Cansockets plugin missing", LogType::AppError);

    QObject::connect(heartbeatTimer, &QTimer::timeout, this, &CanHandler::heartbeat);
}

CanHandler::~CanHandler() {
    delete canDevice;
    delete heartbeatTimer;
}

bool CanHandler::connect()
{
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("vcan0"), &errorString);
    //todo: remember to change before building for pi
    if (!canDevice) {
        logger.add("Can Device creation failed: " + errorString, LogType::Critical);
        return false;
    }
    if (!canDevice->connectDevice()) {
        logger.add("Can Device created, but failed to connect", LogType::Critical);
        return false;
    }
    else
        logger.add("Can Device created successfully");
    QObject::connect(canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::onCanFrameReceived);
    QObject::connect(canDevice, &QCanBusDevice::errorOccurred, this, &CanHandler::onCanErrorOcurred);

    return true;
}

inline bool CanHandler::connected()
{
//    return canDevice->state() == QCanBusDevice::ConnectedState;   //fixme: SIGSEGV
    return true;
}

bool CanHandler::send(const QCanBusFrame &toSend)
{
    if (not(connected()))
        return false;

    if (canDevice->writeFrame(toSend))
        return true;

    return false;
}

void CanHandler::startNewDataCycle()
{
    for (auto &device: canData.synchronousFrames)
        device->hasBeenUpdated = false;
}

DeviceBase *CanHandler::getAsyncFrame()
{
    if (asyncFrames.empty())
        return nullptr;

    auto dev = asyncFrames.front();
    asyncFrames.pop();
    return dev;
}

void CanHandler::onCanFrameReceived()
{
    QCanBusFrame frame = canDevice->readFrame();

    logger.addCAN(frame.toString());

    for (auto dev: canData.synchronousFrames) {
        if (dev->id == frame.frameId()) {
            std::memcpy(dev->dataPtr, frame.payload().constData(), dev->dlc);
            dev->hasBeenUpdated = true;
            return;
        }
    }

    for (auto event: canData.asynchronousFrames) {
        if (event->id == frame.frameId()) {
            std::memcpy(event->dataPtr, frame.payload().constData(), event->dlc);
        }
    }

}

void CanHandler::onCanErrorOcurred()
{
    auto const error = canDevice->errorString();
    logger.add("Can Error detected: " + error);
}

void CanHandler::heartbeat() {
    QCanBusFrame heartbeatFrame;
    heartbeatFrame.setFrameId(DASH_MAIN_CAN_ID);

    Dash_Main dash_main;
    dash_main.device_state = this->heartBeatState;

    char * data = new char[sizeof(Dash_Main)];

    std::memcpy(data, &dash_main, sizeof(Dash_Main));

    heartbeatFrame.setPayload(data);

    send(heartbeatFrame);

    delete[] data;
}
