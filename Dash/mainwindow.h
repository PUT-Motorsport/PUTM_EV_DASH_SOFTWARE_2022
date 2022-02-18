#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "guicomponent.h"

#include "canhandler.h"
#include "vehicle.h"
#include "logger.h"
#include "dvselect.h"
#include "servicemode.h"

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

    GUIComponent * subwindowShown;      //ensures two-way communication between classes
public slots:
    void updateData(Parameter param, qreal value);
    void raiseError(int errorCode, QString const &errorMessage);  //TODO: add data types and implementation
    void navigate(Navigation pressed);

private slots:
    void reopen();

private:
    Ui::MainWindow *ui;

    CanHandler * canHandler;
    DvSelect * dvSelect;
    ServiceMode * serviceMode;

    Status canStatus;
    void updateCANStatus(Status newStatus);

    int errorCounter;

};
#endif // MAINWINDOW_H
