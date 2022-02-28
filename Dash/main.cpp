#include "mainwindow.h"
#include "logger.h"
#include <QApplication>
#include <QFile>

Logger logger;
CanHandler canHandler;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleSheetFile (":/style.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet {QLatin1String(styleSheetFile.readAll())};
    a.setStyleSheet(styleSheet);
    MainWindow w;
    w.show();
    return a.exec();
}
