#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrivingSelect)
{
    ui->setupUi(this);
}

DrivingSelect::~DrivingSelect()
{
    delete ui;
}
