#include "dvselect.h"
#include "ui_dvselect.h"

DvSelect::DvSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DvSelect)
{
    ui->setupUi(this);
}

DvSelect::~DvSelect()
{
    delete ui;
}
