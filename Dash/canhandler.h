#ifndef CANHANDLER_H
#define CANHANDLER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QFile>
#include <QDomDocument>
#include <QTimer>

#include "logger.h"
#include "vehicle.h"

extern Logger logger;
enum class HeartbeatType{Normal, Driverless};

class CanHandler : public QObject
{
    Q_OBJECT
public:

    explicit CanHandler(QObject *parent = nullptr);
    ~CanHandler();
    bool connect();
    bool send(QCanBusFrame const &toSend);
    void setHeartbeat(HeartbeatType type) {heartbeatType = type;}

signals:

    void updateGUI(Parameter param, qreal newValue);
    void raiseError(QString const &errorMessage, int errorCode = -1);
    void navigation(Navigation pressed);
    void getConfirmation(QDomElement const &data, QString value);

private slots:

    void onCanFrameReceived();
    void onCanErrorOcurred();   //TODO: handling of can errors

private:

    HeartbeatType heartbeatType;
    void heartbeat();
    QTimer *heartbeatTimer;
    static constexpr int heartbeatFrequency = 1;    //to be changed in release
    static constexpr int heartbeatID = 0x16F;   //example value

    QDomElement frameParserInfo;        //split into CAN parser???
    QString const xmlFile = QStringLiteral("../ProgramData/incomingFrames.xml");
    QCanBusDevice * canDevice;

    void parseError(QCanBusFrame const &toParse, QDomElement const &parserInfo);
    void parseNavigation(QCanBusFrame const &toParse);
    void parseUpdate(QCanBusFrame const &toParse, QDomElement const &parserInfo);
    void parseConfirmation(QCanBusFrame const &toParse, QDomElement const &parserInfo);

    qreal frameValue(QByteArray const &toCast) const;
};

#endif // CANHANDLER_H
