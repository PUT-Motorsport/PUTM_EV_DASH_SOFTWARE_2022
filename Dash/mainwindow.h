#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vehicle.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//remember to zero all values on screens
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void updateData(Parameter param, qreal value);
    void raiseError();  //add data types and implementation
private:
    Ui::MainWindow *ui;
    int speed;
    int temperature;
    int rpm;
    int soc;
    int power;
    Status canStatus;
    QStringList warnings, erorrs;

};
#endif // MAINWINDOW_H
