#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QDebug>
#include <QTime>
#include <QFile>
#include <QDomDocument>

enum class LogType {Error, Critical, AppError, Normal};

class Logger {
public:
    static void loadXML(QDomElement &target, QString const &fileName);

    static void open();
    static void add(QString const &message, LogType type = LogType::Normal);    //should this class be a singleton?
    static void addCAN(QString canFrame);
    static void close();

    static QString logLine; //for loading lines from external functions
    static QString canLine;
private:
    static QString const canLogName;
    static QString const logName;

    static QFile canLog;
    static QFile log;

    static QTextStream canStream;
    static QTextStream logStream;
};

#endif // LOGGER_H
