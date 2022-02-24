#include "canhandler.h"

CanHandler::CanHandler(QObject *parent)
    : QObject{parent}
{
    if (!QCanBus::instance()->plugins().contains("socketcan"))
        Logger::add("Cansockets plugin missing", LogType::AppError);
    Logger::loadXML(frameParserInfo, xmlFile);
}

bool CanHandler::connect()
{
    QString errorString;
    canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                  QStringLiteral("vcan0"), &errorString);   //remember to change before building for pi
    if (!canDevice) {
        Logger::add("Can Device creation failed: " + errorString, LogType::Critical);
        return false;
    }
    if (!canDevice->connectDevice()) {
        Logger::add("Can Device created, but failed to connect", LogType::Critical);
        return false;
    }
    else
        Logger::add("Can Device created successfully");
    QObject::connect(canDevice, &QCanBusDevice::framesReceived, this, &CanHandler::onCanFrameReceived);
    return true;
}

bool CanHandler::send(QCanBusFrame const &toSend)
{
    if (not(canDevice->state() == QCanBusDevice::CanBusDeviceState::ConnectedState)) {
        Logger::add("Attempted to send a frame to can which isn't open", LogType::Critical);
        emit raiseError("Can isn't open");
        return false;
    }
    if (not(canDevice->writeFrame(toSend))) {
        Logger::add("CAN is connected, but failed to relay a message", LogType::Critical);
        emit raiseError("CAN connected, but failed to relay a message");
        return false;
    }
    return true;
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
        if (element.attribute("id").toInt(nullptr, 16) == frameID)
            break;
        currentSearch = currentSearch.nextSibling();
   }
    if (currentSearch.isNull()) {
        Logger::add("Search failed: no frame with corresponding ID", LogType::AppError);
        return;
    }
    QDomElement passableElement = currentSearch.toElement();
    int frameType = passableElement.attribute("type").split(":").at(0).toInt();
    switch (static_cast<FrameType>(frameType)) {
    case FrameType::error:
        parseError(newFrame, passableElement);    //FIX ME ???will this get deleted before I access it??? (possible bug)
        break;
    case FrameType::update:
        parseUpdate(newFrame, passableElement);
        break;
    case FrameType::navigation:
        parseNavigation(newFrame);
        break;
    case FrameType::confirmation:
        parseConfirmation(newFrame, passableElement);
        break;
    default:
        Logger::add("Parser failure: Wrong argument", LogType::AppError);
    }
}

void CanHandler::parseError(QCanBusFrame const &toParse, QDomElement const &parserInfo)
{
    QString errorDesc = parserInfo.attribute("logger");
    int errorCode = frameValue(toParse.payload());
    if (errorDesc != "")
        Logger::add(QString::number(errorCode) + errorDesc, LogType::Error);
    else
        Logger::add("Unknown error, ref number " + QString::number(errorCode), LogType::Critical);
    emit raiseError(errorDesc, errorCode);
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
    QString logInfo = parserInfo.attribute("logger");
    if (logInfo != "") {
        Logger::add(logInfo);
    }
    qreal newValue = frameValue(toParse.payload());
    emit updateGUI(updated, newValue);
}

void CanHandler::parseConfirmation(const QCanBusFrame &toParse, const QDomElement &parserInfo)
{
    QString value = QString::number(frameValue(toParse.payload()));
    emit getConfirmation(parserInfo, value);
}

qreal CanHandler::frameValue(QByteArray const &data) const
{
    //probably a way to find values with a multiplier
    bool conversionStatus;
    QString bytesString = QLatin1String(data.toHex().toUpper());
    qreal frameValue = bytesString.toInt(&conversionStatus, 16);
    if (not(conversionStatus)) {
        qDebug() << "Debug: Variable conversion failed";
        return -1;
    }
    return frameValue;
}
