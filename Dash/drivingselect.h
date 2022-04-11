#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>
#include <QCanBusFrame>
#include <QRegularExpression>
#include <QTimer>

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
    explicit DrivingSelect(QWidget *parent = nullptr);
    ~DrivingSelect();
    void navigate(buttonStates navigation) override;
    void raiseError(QString const &errorMessage) override;

private:
    void changeHighlight();
    void sendCANFrame();
    void changeValue();
    void loadSettings();
    void changeAppsCurve();
    void resetToCurrent();

    Ui::DrivingSelect *ui;
    Setting current;
    QFile valuesFile;

    QStringList regainValues;
    QStringList tractionValues;
    QStringList powerValues;
    QStringList fanValues;
    QList<QStringList> payloads;
    int appsCurve;                  //will be displayed as an image

    QStringList currentSettingsValues;
};

#endif // DRIVINGSELECT_H
