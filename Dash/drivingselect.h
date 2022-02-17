#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>

#include <QKeyEvent>

#include "logger.h"

enum class Setting {Regain, Traction, Power, Fan, Apps};

namespace Ui {
class DrivingSelect;
}

class DrivingSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DrivingSelect(QWidget *parent = nullptr);  //TODO: CAN support
    ~DrivingSelect();
protected:
    void keyPressEvent(QKeyEvent *event) override;      //testing purposes only
private:
    Ui::DrivingSelect *ui;
    Setting current;

    void changeHighlight();
};

#endif // DRIVINGSELECT_H
