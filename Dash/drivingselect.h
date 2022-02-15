#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QDialog>

namespace Ui {
class DrivingSelect;
}

class DrivingSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DrivingSelect(QWidget *parent = nullptr);
    ~DrivingSelect();
    //will the driving select send data?
private:
    Ui::DrivingSelect *ui;
};

#endif // DRIVINGSELECT_H
