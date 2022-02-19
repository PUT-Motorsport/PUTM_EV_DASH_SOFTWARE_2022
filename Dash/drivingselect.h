#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>
#include <QCanBusFrame>
#include <QRegularExpression>
#include "guicomponent.h"
#include "canhandler.h"
#include "logger.h"

enum class Setting {Regain, Traction, Power, Fan, Apps};

namespace Ui {
class DrivingSelect;
}

class DrivingSelect : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit DrivingSelect(CanHandler * can, QWidget *parent = nullptr);  //TODO: CAN support
    ~DrivingSelect();
    void navigate(Navigation pressed) override;
    void raiseError(int errorCode, QString const &errorMessage) override;

private:
    void changeHighlight();
    void sendCANFrame();
    void changeValue();
    void loadSettings();
    void changeAppsCurve();

    Ui::DrivingSelect *ui;
    Setting current;
    CanHandler * can;
    QFile valuesFile;
    QStringList regainValues;
    QStringList tractionValues;
    QStringList powerValues;
    QStringList fanValues;
    int appsCurve;                  //will be displayed as an image
    QList<QStringList> payloads;
};

#endif // DRIVINGSELECT_H
