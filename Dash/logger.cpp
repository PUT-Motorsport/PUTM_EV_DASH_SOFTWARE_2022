#include "logger.h"

void Logger::loadXML(QDomElement &target, QString const &fileName)
{
    QDomDocument xmlDocument("xmlDocument");        //to be rewritten, but makes debug easier
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
    //will open logger file
    qDebug() << "Logger file created";
}

void Logger::add(const QString &message, LogType type)
{
    //will append logger file
    switch (type) {
    case LogType::Normal:
        qDebug() << QTime().currentTime().toString() + ' ' + message;
        break;
    case LogType::AppError:
        qDebug() << QTime().currentTime().toString() +  " @App Error@ " + message;
        break;
    case LogType::Error:
        qDebug() << QTime().currentTime().toString() + "! Error! " + message;
        break;
    case LogType::Critical:
        qDebug() << QTime().currentTime().toString() + " !!Fatal Error!! " + message;
        break;
    }
}

void Logger::addCAN(QString canFrame)
{
    //will append can logger file
    qDebug() << QTime().currentTime().toString() + " Can: " + canFrame;
}

void Logger::close()
{
    //will close and rename the logger file
    qDebug() << "Logger file closed";
}
