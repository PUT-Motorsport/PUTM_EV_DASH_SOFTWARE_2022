#ifndef DVSELECT_H
#define DVSELECT_H

#include <QDialog>
#include <QCanBusDevice>
#include <QCanBusFrame>
namespace Ui {
class DvSelect;
}

class DvSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DvSelect(QWidget *parent = nullptr);  //needs to be able to send frames
    ~DvSelect();

private:
    Ui::DvSelect *ui;
    QList<QString> missionList;
    //how the frames will be stored???
};

#endif // DVSELECT_H
