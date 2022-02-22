#include "logger.h"

QString const Logger::canLogName = QStringLiteral("can.txt");   //TODO: Move to a different subdirectory
QString const Logger::logName = QStringLiteral("log.txt");

QFile Logger::canLog;
QFile Logger::log;
QTextStream Logger::canStream;
QTextStream Logger::logStream;

void Logger::loadXML(QDomElement &target, QString const &fileName)
{
    QDomDocument xmlDocument("xmlDocument");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::add("XML file could not be opened", LogType::AppError);
        return;
    }
    if (!xmlDocument.setContent(&file)) {
        Logger::add("XML file opened, but internal parsing failed", LogType::AppError);
        file.close();
        return;
    }
    file.close();   //file loaded and can be closed
    Logger::add("xml File loaded.");
    target = xmlDocument.documentElement();

}

void Logger::open()
{
    qDebug() << "Logger file created";

    Logger::canLog.setFileName(canLogName);

    if (not(Logger::canLog.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)))
        qDebug() << "Failed to open can log file";
    Logger::canStream.setDevice(&Logger::canLog);

    log.setFileName(Logger::logName);

    if (not(log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)))
        qDebug() << "Failed to open log file";
    logStream.setDevice(&log);

    QString header = "...System started at: " + QTime().currentTime().toString() + "...";
    logStream << header;
    canStream << header;
}

void Logger::add(const QString &message, LogType type)
{
    switch (type) {
    case LogType::Normal:
        qDebug() << QTime().currentTime().toString() + ' ' + message;
        logStream << QTime().currentTime().toString() + ' ' + message + '\n';
        break;
    case LogType::AppError:
        qDebug() << QTime().currentTime().toString() +  " @App Error@ " + message;
        logStream << QTime().currentTime().toString() +  " @App Error@ " + message + '\n';
        break;
    case LogType::Error:
        qDebug() << QTime().currentTime().toString() + " !Error! " + message;
        logStream << QTime().currentTime().toString() +  " !Error! " + message + '\n';
        break;
    case LogType::Critical:
        qDebug() << QTime().currentTime().toString() + " !!Fatal Error!! " + message;
        logStream << QTime().currentTime().toString() + " !!Fatal Error!! " + message + '\n';
        break;
    }
}

void Logger::addCAN(QString canFrame)
{
    qDebug() << QTime().currentTime().toString() + " Can: " + canFrame;
    canStream << QTime().currentTime().toString() + ' ' + canFrame + '\n';
}

void Logger::close()
{
    qDebug() << "Logger file closed";
    canLog.rename(QTime().currentTime().toString() + "-can.txt");
    log.rename(QTime().currentTime().toString() + "-log.txt");
    canLog.close();
    log.close();
}
