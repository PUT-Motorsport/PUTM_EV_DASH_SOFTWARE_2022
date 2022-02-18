#ifndef DVSELECT_H
#define DVSELECT_H

#include <QDialog>
#include <QCanBusDevice>
#include <QCanBusFrame>

#include "guicomponent.h"

namespace Ui {
class DvSelect;
}

class DvSelect : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit DvSelect(QWidget *parent = nullptr);  //needs to be able to send frames
    ~DvSelect();

    void navigate(Navigation pressed) override;
    void raiseError(int errorCode, QString const &errorMessage) override;

private:
    Ui::DvSelect *ui;
    //how the frames will be stored???
};

#endif // DVSELECT_H
