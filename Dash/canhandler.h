#ifndef CANHANDLER_H
#define CANHANDLER_H

#include <QObject>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QFile>
#include <QDomDocument>

#include "logger.h"
#include "vehicle.h"
class CanHandler : public QObject
{
    Q_OBJECT
public:
    explicit CanHandler(QObject *parent = nullptr);
    bool connect();
    bool send(QCanBusFrame const &toSend);
signals:

    void updateGUI(Parameter param, qreal newValue);
    void raiseError(QString const &errorMessage, int errorCode = -1);
    void navigation(Navigation pressed);
    void getConfirmation(QDomElement const &data, QString value);

private slots:
    void onCanFrameReceived();
    void onCanErrorOcurred();   //TODO: handling of can errors
private:

    QDomElement frameParserInfo;
    QString const xmlFile = QStringLiteral("../ProgramData/incomingFrames.xml");
    QCanBusDevice * canDevice;

    void parseError(QCanBusFrame const &toParse, QDomElement const &parserInfo);
    void parseNavigation(QCanBusFrame const &toParse);
    void parseUpdate(QCanBusFrame const &toParse, QDomElement const &parserInfo);
    void parseConfirmation(QCanBusFrame const &toParse, QDomElement const &parserInfo);

    qreal frameValue(QByteArray const &toCast) const;
};

#endif // CANHANDLER_H
