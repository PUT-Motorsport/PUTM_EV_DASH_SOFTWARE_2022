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
    void connect();
    void send();    //where should the frame characteristics be loaded from file???
signals:
    void updateGUI(struct Update update);
    void raiseError(struct Error error);
    void navigation(Button pressed);
private slots:
    void onCanFrameReceived() const;
private:
    void loadXML();
    QDomElement frameParserInfo;
    QString const xmlFile = QStringLiteral("test.xml");
    QCanBusDevice * canDevice;

    void parseError(int id, QByteArray payload) const;
    void parseNavigation(int id, QByteArray payload) const;
    void parseUpdate(int id, QByteArray payload) const;
};

#endif // CANHANDLER_H
