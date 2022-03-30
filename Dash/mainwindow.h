#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>

#include "guicomponent.h"

#include "canhandler.h"
#include "vehicle.h"
#include "dvselect.h"
#include "servicemode.h"
#include "changeconfirm.h"

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
    GUIComponent * interruptSubwindowShown;     //allows to store previously open window if change confirmation "interrupts"
public slots:
    void updateData(Parameter param, qreal value);
    void raiseError(QString const &errorMessage, int errorCode = -1);       //TODO: remove error codes functionality
    void navigate(Navigation pressed);
    void getConfirmation(QDomElement const &data, QString value);

private slots:
    void reopen();

private:

    void updateBestTime();
    void resetTimer();

    int m_speed;        //kept so that a change can start a timer
    QTime best;
    QElapsedTimer * elapsedTimer;
    bool timerStarted;
    QTimer updateTimer;     //will call the gui to update timer

    Ui::MainWindow *ui;

    DvSelect * dvSelect;
    ServiceMode * serviceMode;

    ChangeConfirm * changeConfirm;

    int errorCounter;

};
#endif // MAINWINDOW_H
