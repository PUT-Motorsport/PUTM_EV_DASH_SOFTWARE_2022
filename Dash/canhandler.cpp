#include "canhandler.h"

CanHandler::CanHandler(QObject *parent)
    : QObject{parent}
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        Logger::add("Cansockets plugin missing");
    Logger::loadXML(frameParserInfo, xmlFile);
}

bool CanHandler::connect()
{
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("vcan0"), &errorString);   //remember to change before building for pi
    if (!canDevice) {
        Logger::add("App Error: Can Device creation failed: " + errorString);
        return false;
    }
    if (!canDevice->connectDevice()) {
        Logger::add("Fatal app Error: Can Device created, but failed to connect");
        return false;
    }
    else
        Logger::add("Can Device created successfully");
    QObject::connect(canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::onCanFrameReceived);
    return true;
}

void CanHandler::send(QCanBusFrame const &toSend) const
{
    if (not(canDevice->state() == QCanBusDevice::CanBusDeviceState::ConnectedState)) {
        Logger::add("App Error: Attempted to send a frame to can which isn't open");
        return;
    }
    if (not(canDevice->writeFrame(toSend))) {
        Logger::add("CAN Error: CAN is connected, but failed to relay a message");
    }
}

void CanHandler::onCanFrameReceived()
{
    QCanBusFrame newFrame = canDevice->readFrame();
    Logger::addCAN(newFrame.toString());
    //first phase of parsing, looking for correct handoff
    int frameID = newFrame.frameId();
    QDomNode currentSearch = frameParserInfo.firstChild();
    while (not(currentSearch.isNull())) {
        QDomElement element = currentSearch.toElement();
        if (element.attribute("id").toInt() == frameID)
            break;
        currentSearch = currentSearch.nextSibling();
   }
    if (currentSearch.isNull()) {
        Logger::add("App Error: Search failed: no frame with corresponding ID");
        return;
    }
    QDomElement passableElement = currentSearch.toElement();
    int frameType = passableElement.attribute("type").split(":").at(0).toInt();
    switch (static_cast<FrameType>(frameType)) {
    case FrameType::error:
        parseError(newFrame, passableElement);    //will this get deleted before I access it??? (possible bug)
        break;
    case FrameType::update:
        parseUpdate(newFrame, passableElement);
        break;
    case FrameType::navigation:
        parseNavigation(newFrame);
        break;
    default:
        Logger::add("App Error: Parser failure: Wrong argument");
    }
}

void CanHandler::parseError(QCanBusFrame const &toParse, QDomElement const &parserInfo)
{
    QString errorDesc = parserInfo.attribute("logger");
    int errorCode = frameValue(toParse.payload());
    if (errorDesc != "")
        Logger::add(QString::number(errorCode) + errorDesc);
    emit raiseError(errorCode, errorDesc);
}

void CanHandler::parseNavigation(QCanBusFrame const &toParse)
{
    int pressedInt = static_cast<int>(frameValue(toParse.payload()));
    emit navigation(static_cast<Navigation>(pressedInt));
}

void CanHandler::parseUpdate(QCanBusFrame const &toParse, QDomElement const &parserInfo)
{
    //take the numerical prefix and cast it to a parameter:
    Parameter updated = static_cast<Parameter>(parserInfo.attribute("parameter").split(":").at(0).toInt());
    qreal newValue = frameValue(toParse.payload());
    emit updateGUI(updated, newValue);
}

qreal CanHandler::frameValue(QByteArray const &data) const
{
    //probably a way to find values with a multiplier
    bool conversionStatus;
    QString bytesString = QLatin1String(data.toHex().toUpper());
    qDebug() << bytesString;
    qreal frameValue = bytesString.toInt(&conversionStatus);
    if (not(conversionStatus)) {
        qDebug() << "App Error: Variable conversion failed";
        return -1;
    }
    return frameValue;
}
