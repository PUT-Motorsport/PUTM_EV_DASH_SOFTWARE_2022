#include "logger.h"

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
