#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include <QDialog>

#include "guicomponent.h"
#include "drivingselect.h"
#include "canraw.h"
#include "logs.h"

#include "vehicle.h"
#include "logger.h"

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
    void navigate(Navigation pressed) override;
    void raiseError(int errorCode, QString const &errorMessage) override;

private:
    Ui::ServiceMode *ui;
    DrivingSelect * driving;
    Logs * logs;
    CanRaw * canRaw;
    GUIComponent * subwindowShown;
};

#endif // SERVICEMODE_H
