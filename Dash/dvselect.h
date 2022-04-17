#ifndef DVSELECT_H
#define DVSELECT_H

#include <QDialog>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDomDocument>
#include <QTimer>

#include "guicomponent.h"
#include "canhandler.h"

namespace Ui {
class DvSelect;
}

//deprecated

class DvSelect : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit DvSelect(QWidget *parent = nullptr);
    ~DvSelect();

    void navigate(buttonStates navigation) override;
    void raiseError(QString const &errorMessage) override;

private:
    void toggleMission(int direction);
    void sendCANframe();

    Ui::DvSelect *ui;
    QDomElement missionsFile;
    QStringList missionNames;
    int missionCount;
    int currentMission;

};

#endif // DVSELECT_H
