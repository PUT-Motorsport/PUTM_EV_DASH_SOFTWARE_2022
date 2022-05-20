#include "canhandler.h"

CanHandler::CanHandler(QObject *parent): QObject(parent),
    heartbeatTimer(new QTimer())
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        logger.add("Cansockets plugin missing", LogType::AppError);

    QObject::connect(heartbeatTimer, &QTimer::timeout, this, &CanHandler::heartbeat);

#ifdef SEND_HEARTBEAT
    heartbeatTimer->start(1 / heartbeatFrequency);
#endif

}

CanHandler::~CanHandler() {
    delete canDevice;
    delete heartbeatTimer;
}

bool CanHandler::connect()
{
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("can0"), &errorString);
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

bool CanHandler::connected()
{
    return canDevice->state();
}

bool CanHandler::send(const QCanBusFrame &toSend)
{
    if (not(connected())) {
        logger.add("Attempted sending in unconnected state", LogType::Critical);
        return false;
    }
    if (canDevice->writeFrame(toSend))
        return true;

    logger.add("Frame sending failed");
    return false;
}

void CanHandler::startNewDataCycle()
{
    for (auto &device: canData.synchronousFrames)
        device->hasBeenUpdated = false;
}

void CanHandler::onCanFrameReceived()
{
    QCanBusFrame frame = canDevice->readFrame();

    logger.addCAN(frame.toString());

    for (auto dev: canData.synchronousFrames) {
        if (dev->id == frame.frameId()) {
            canData.mtx.lock();
            std::memcpy(dev->dataPtr, frame.payload().constData(), dev->dlc);
            dev->hasBeenUpdated = true;
            canData.mtx.unlock();
            return;
        }
    }

    for (auto event: canData.asynchronousFrames) {
        if (event->id == frame.frameId()) {
            canData.mtx.lock();
            std::memcpy(event->dataPtr, frame.payload().constData(), event->dlc);
            event->hasBeenUpdated = true;
            canData.mtx.unlock();
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


    heartbeatFrame.setPayload(QByteArray(reinterpret_cast<char *>(&dash_main), sizeof(dash_main)));

    send(heartbeatFrame);
}
