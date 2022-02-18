#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>

#include <QKeyEvent>

#include "guicomponent.h"

#include "logger.h"

enum class Setting {Regain, Traction, Power, Fan, Apps};

namespace Ui {
class DrivingSelect;
}

class DrivingSelect : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit DrivingSelect(QWidget *parent = nullptr);  //TODO: CAN support
    ~DrivingSelect();
    void navigate(Navigation pressed) override;
    void raiseError(int errorCode, QString const &errorMessage) override;
protected:
    void keyPressEvent(QKeyEvent *event) override;      //testing purposes only
private:
    Ui::DrivingSelect *ui;
    Setting current;

    void changeHighlight();
};

#endif // DRIVINGSELECT_H
