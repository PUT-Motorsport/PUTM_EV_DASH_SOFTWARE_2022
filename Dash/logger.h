#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QDebug>
#include <QTime>
#if 0
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
    static void open();
    static void add(QString message);
    static void addCAN(QString canFrame);
    static void close();
};
#endif
#endif // LOGGER_H
