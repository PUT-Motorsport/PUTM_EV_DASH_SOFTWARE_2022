#include "logger.h"

QString const Logger::canLogName = QStringLiteral("can.txt");   //TODO: Move to a different subdirectory
QString const Logger::logName = QStringLiteral("log.txt");

QString Logger::logLine;
QString Logger::canLine;

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
    logLine = QTime().currentTime().toString();
    switch (type) {
    case LogType::Normal:
        break;
    case LogType::AppError:
        logLine += " @App Error@ ";
        break;
    case LogType::Error:
        logLine += " !Error! ";
        break;
    case LogType::Critical:
        logLine += " !!!Critical Error!!! ";
        break;
    }
    logLine += message;
    qDebug() << logLine;
    logStream << logLine << '\n';
}

void Logger::addCAN(QString canFrame)
{
    canLine = QTime().currentTime().toString() + ' ' + canFrame;
    qDebug() << canLine;
    canStream << canLine + '\n';
}

void Logger::close()
{
    qDebug() << "Logger file closed";
    canLog.rename(QTime().currentTime().toString() + "-can.txt");
    log.rename(QTime().currentTime().toString() + "-log.txt");
    canLog.close();
    log.close();
}
