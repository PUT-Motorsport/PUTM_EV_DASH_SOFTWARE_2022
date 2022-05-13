#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>
#include <QCanBusFrame>
#include <QRegularExpression>
#include <QTimer>
#include <QLabel>

#include "lib/CRC.h"
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
    uint8_t getSettingValue(Setting setting, bool next);
    void changeSettingValue(bool next);

    Setting currentSetting; //todo: rename
    Dash_TCS previousSettings;
    Dash_TCS currentSettings;

    Ui::DrivingSelect *ui;
    QList<QPair<QLabel *, QLabel *>> labels;

    QString const getSettingName(Setting setting, uint8_t value);

    QVector<QString> presets;
    QString const presetsFileName{"presets.csv"};
    QVector<QString> descriptions;
    QString const descriptionsFileName{"names.txt"};
    QVector<QString> values;
    QString const valuesFileName{"settings.csv"};

    QVector<QString> loadFile(QString const &fileName);

    //crc
    CRC::Table<uint8_t, sizeof(uint8_t)> table;
};

#endif // DRIVINGSELECT_H
