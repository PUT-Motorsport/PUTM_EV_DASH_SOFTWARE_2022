#include "canhandler.h"

CanHandler::CanHandler(QObject *parent)
    : QObject{parent}
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        Logger::add("Cansockets plugin missing");

}

void CanHandler::connect()
{
    loadXML();
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("vcan0"), &errorString);
    if (!canDevice) {
        Logger::add("Can Device creation failed: " + errorString);
        return;
    }
    if (!canDevice->connectDevice()) {
        Logger::add("Can Device created, but failed to connect");
        return;
    }
    else
        Logger::add("Can Device created successfully");
    QObject::connect(canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::onCanFrameReceived);
}

void CanHandler::onCanFrameReceived() const
{
    QCanBusFrame newFrame = canDevice->readFrame();
    Logger::addCAN(newFrame.toString());
    //first phase of parsing, looking for correct handoff
    int frameID = newFrame.frameId();
    QByteArray payload = newFrame.payload();
    switch (frameID) {
    case FrameType::update:
        parseUpdate(frameID, payload);
        break;
    case FrameType::error:
        parseError(frameID, payload);
        break;
    case FrameType::navigation:
        parseNavigation(frameID, payload);
        break;
    }
}

void CanHandler::loadXML()
{
    QDomDocument xmlDocument("xmlDocument");
    QFile file(xmlFile);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::add("XML file could not be opened");
        return;
    }
    if (!xmlDocument.setContent(&file)) {
        Logger::add("XML file opened, but internal parsing failed");
        file.close();
        return;
    }
    file.close();   //file loaded and can be closed
    frameParserInfo = xmlDocument.documentElement();

}

void CanHandler::parseError(int id, QByteArray payload) const
{
    return;
}

void CanHandler::parseNavigation(int id, QByteArray payload) const
{
    return;
}

void CanHandler::parseUpdate(int id, QByteArray payload) const
{
    return;
}
