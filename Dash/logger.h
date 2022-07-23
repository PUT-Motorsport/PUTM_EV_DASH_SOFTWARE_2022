#ifndef LOGGER_H
#define LOGGER_H
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QTime>
#include <iostream>

enum class LogType { Error, Critical, AppError, Normal };

class Logger {
 public:
  Logger();
  ~Logger();
  void forceFlush();
  void open();
  void add(QString const &message, LogType type = LogType::Normal);
  void addCAN(QString canFrame);

  QString logLine;  // for loading lines from external functions
  QString canLine;
  unsigned int logLinesCount;

 private:
  QString const canLogName;
  QString const logName;

  QFile canLog;
  QFile log;

  QTextStream canStream;
  QTextStream logStream;
};

#endif  // LOGGER_H
