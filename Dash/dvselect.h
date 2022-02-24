#ifndef DVSELECT_H
#define DVSELECT_H

#include <QDialog>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDomDocument>
#include <QTimer>

#include "logger.h"

#include "guicomponent.h"
#include "canhandler.h"

namespace Ui {
class DvSelect;
}

class DvSelect : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit DvSelect(CanHandler * can, QWidget *parent = nullptr);
    ~DvSelect();

    void navigate(Navigation pressed) override;
    void raiseError(QString const &errorMessage, int errorCode = -1) override;

private:
    void toggleMission(int direction);
    void sendCANframe();

    Ui::DvSelect *ui;
    QString const pathToXML = QStringLiteral("../ProgramData/dv.xml");
    QDomElement missionsFile;
    QStringList missionNames;
    int missionCount;
    int currentMission;

    CanHandler * can;
};

#endif // DVSELECT_H
