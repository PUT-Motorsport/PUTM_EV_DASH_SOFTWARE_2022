#include <QApplication>
#include <QFile>
#include <QProcess>

#include "logger.h"
#include "mainwindow.h"

Logger logger;
CanHandler canHandler;

int main(int argc, char *argv[]) {
  QProcess::execute("xset", QStringList() << "-dpms" << "s" << "off");
  qDebug()
      << "This is a debug build";  // will print only compiled in debug mode
  // qt metatype registration
  qRegisterMetaType<Parameter>("Parameter");
  qRegisterMetaType<buttonStates>("buttonStates");
  qRegisterMetaType<Side>("Side");
  qRegisterMetaType<scrollStates>("scrollStates");
  qRegisterMetaType<BMS_HV_states>("BMS_HV_states");

  QApplication a(argc, argv);
  QFile styleSheetFile(":/style.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet{QLatin1String(styleSheetFile.readAll())};
  a.setStyleSheet(styleSheet);
  MainWindow w;
  w.show/*FullScreen*/();
  return a.exec();
}
