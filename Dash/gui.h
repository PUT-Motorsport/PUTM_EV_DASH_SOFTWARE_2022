#ifndef GUI_H
#define GUI_H

#include <QObject>
#include "mainwindow.h"
#include "canraw.h"
#include "drivingselect.h"
#include "dvselect.h"
#include "logs.h"
#include "canhandler.h"
#include "servicemode.h"
#include "changeconfirm.h"

#include "vehicle.h"
#include "logger.h"
class GUI : public QObject
{
    Q_OBJECT
public:
    explicit GUI(QObject *parent = nullptr);
    ~GUI();
public slots:

    void updateGUI(Parameter param, qreal newValue);
    void raiseError(int errorCode, QString errorText);
    void navigate(Navigation button);

signals:
private:
    void changeWindow(Window newWindow);

    CanHandler * canHandler;
    QDialog* currentWindowPtr;  //enables easy window hiding through inheritance
    Window currentWindow;

    MainWindow * mainWindow;
    CanRaw * canRaw;
    DrivingSelect * drivingSelect;
    Logs * logs;
    DvSelect * dvSelect;
    ServiceMode * serviceMode;
    ChangeConfirm * changeConfirm;
    
    const QList<Parameter> onMainWindow = {Parameter::Speed, Parameter::Power, Parameter::CoolantTemp,
                                          Parameter::SOC, Parameter::RPM};
};

#endif // GUI_H
