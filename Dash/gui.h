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

#include "vehicle.h"
#include "logger.h"
class GUI : public QObject
{
    Q_OBJECT
public:
    explicit GUI(QObject *parent = nullptr);
    ~GUI();
public slots:

    void updateGUI(Update update);
    void raiseError(Error error);
    void navigate(Button button);

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
};

#endif // GUI_H
