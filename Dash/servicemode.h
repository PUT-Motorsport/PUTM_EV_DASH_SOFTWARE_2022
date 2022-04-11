#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include <QDialog>
#include <QTimer>

#include "guicomponent.h"
#include "drivingselect.h"
#include "canraw.h"
#include "logs.h"

#include "vehicle.h"
#include "logger.h"
#include "canhandler.h"

namespace Ui {
class ServiceMode;
}

class ServiceMode : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit ServiceMode(QWidget *parent = nullptr);
    ~ServiceMode();
    void updateData(Parameter param, qreal value);
    void navigate(buttonStates navigation) override;
    void raiseError(QString const &errorMessage) override;

private:
    Ui::ServiceMode *ui;
    DrivingSelect * driving;
    Logs * logs;
    CanRaw * canRaw;
    GUIComponent * subwindowShown;
private slots:
    void reopen();
};

#endif // SERVICEMODE_H
