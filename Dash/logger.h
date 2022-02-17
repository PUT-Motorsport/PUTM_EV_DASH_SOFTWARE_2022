#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QDomDocument>

#if 0   //commented out due to qt-specific linker issue, walkaround below
namespace Logger {
void open() {
    //will open log files
}
void close() {
    //will close log files
}
void add(QString logMessage) {
    qDebug() << logMessage;
    //will add to log file
}
void addCAN() {
    //will add to can logger file
}
}
#else
class Logger {
public:
    static void loadXML(QDomElement &target, QString fileName);

    static void open();
    static void add(QString message);   //should there be a way to log errors differently?
    static void addCAN(QString canFrame);
    static void close();
};
#endif
#endif // LOGGER_H
