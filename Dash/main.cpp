#include "logger.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>

Logger logger;
CanHandler canHandler;

int main(int argc, char *argv[]) {
  qDebug() << "This is a debug build"; // will print only compiled in debug mode
  // qt metatype registration
  qRegisterMetaType<Parameter>("Parameter");
  qRegisterMetaType<buttonStates>("buttonStates");
  qRegisterMetaType<Side>("Side");
  qRegisterMetaType<scrollStates>("scrollStates");

  QApplication a(argc, argv);
  QFile styleSheetFile(":/style.qss");
  styleSheetFile.open(QFile::ReadOnly);
  QString styleSheet{QLatin1String(styleSheetFile.readAll())};
  a.setStyleSheet(styleSheet);
  MainWindow w;
  w.showFullScreen();
  return a.exec();
}
