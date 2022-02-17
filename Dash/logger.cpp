#include "logger.h"

void Logger::loadXML(QDomElement &target, QString fileName)
{
    QDomDocument xmlDocument("xmlDocument");        //to be rewritten, but makes debug easier
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::add("Fatal app error: XML file could not be opened");
        return;
    }
    if (!xmlDocument.setContent(&file)) {
        Logger::add("Fatal app error: XML file opened, but internal parsing failed");
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

void Logger::add(QString message)
{
    //will append logger file
    qDebug() << QTime().currentTime().toString() << ' ' << message;
}

void Logger::addCAN(QString canFrame)
{
    //will append can logger file
    qDebug() << QTime().currentTime().toString() << " Can: " << canFrame;
}

void Logger::close()
{
    //will close and rename the logger file
    qDebug() << "Logger file closed";
}
