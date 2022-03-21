#include "canhandler.h"

CanHandler::CanHandler(QObject *parent): QObject(parent), m_connected(false)
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        logger.add("Cansockets plugin missing", LogType::AppError);

}

CanHandler::~CanHandler() {
    delete canDevice;
}

bool CanHandler::connect()
{
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("vcan0"), &errorString);   //remember to change before building for pi
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

    m_connected = true;

    return true;
}

bool CanHandler::send(const QCanBusFrame &toSend)
{
    return false;
}

void CanHandler::onCanFrameReceived()
{
    QCanBusFrame frame = canDevice->readFrame();

    for (auto dev: canData.frameArray) {
        if (dev->id == frame.frameId()) {
            std::memcpy(dev->dataPtr, frame.payload().constData(), dev->dlc);
        }
    }
}

void CanHandler::onCanErrorOcurred()
{
    auto const error = canDevice->errorString();
    logger.add("Can Error detected: " + error);
}


