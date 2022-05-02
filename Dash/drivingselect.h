#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>
#include <QCanBusFrame>
#include <QRegularExpression>
#include <QTimer>
#include <QLabel>

#include "guicomponent.h"
#include "canhandler.h"
#include "logger.h"


enum class Setting {TC, MaxSlipRatio, Algorithm, MaxPower, AppsCurve, RegenPower, Sensitivity};
enum class Algorithm {PI, PID, LQR, LQRI, SDRE, MPC};
enum class Apps_curve {Linear, Wet, Acc};


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

    void setPreset(Side side, scrollStates scroll);
private:

    void sendSettings();
    void changeHighlight();
    void resetToCurrent();
    uint8_t calculateCRC();
    uint8_t getNextSettingValue(Setting setting);
    void changeSettingValue();

    Setting currentSetting; //todo: rename
    Dash_TCS settingsFrame;
    Dash_TCS currentSettings;

    Ui::DrivingSelect *ui;
    QList<QPair<QLabel *, QLabel *>> labels;

    QString const getSettingName(Setting setting, uint8_t value);

    QString const presets{"ProgramData\\presets.csv"};
    QFile presetsFile;
    QString const descriptions{"ProgramData\\names.txt"};
    QFile descriptionsFile;
    QString const values{"ProgramData\\values.csv"};
    QFile valuesFile;
};

#endif // DRIVINGSELECT_H
