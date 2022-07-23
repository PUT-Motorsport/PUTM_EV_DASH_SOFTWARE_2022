#include "logger.h"

Logger::Logger() : canLogName("can.txt"), logName("log.txt") {
  open();
  add("Started at " + QTime().currentTime().toString());
  addCAN("Started at " + QTime().currentTime().toString());
}

Logger::~Logger() {
  qDebug() << "Logger file closed";
  canLog.rename(QTime().currentTime().toString() + "-can.txt");
  log.rename(QTime().currentTime().toString() + "-log.txt");
  canLog.close();
  log.close();
}

void Logger::forceFlush() {
  log.close();
  if (not(log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)))
    qDebug() << "Failed to open log file";
  logStream.setDevice(&log);
}

void Logger::open() {

  canLog.setFileName(canLogName);

  if (not(canLog.open(QIODevice::WriteOnly | QIODevice::Append |
                      QIODevice::Text)))
    qDebug() << "Failed to open can log file";
  canStream.setDevice(&canLog);

  log.setFileName(logName);

  if (not(log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)))
    qDebug() << "Failed to open log file";
  logStream.setDevice(&log);
}

void Logger::add(const QString &message, LogType type) {
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
  logStream << logLine << '\n';
  logLinesCount++;

#ifdef TERMINALOUTPUT
  std::cout << logline;
#endif
}

void Logger::addCAN(QString canFrame) {
  canLine = QTime().currentTime().toString() + ' ' + canFrame;
  canStream << canLine + '\n';
}
