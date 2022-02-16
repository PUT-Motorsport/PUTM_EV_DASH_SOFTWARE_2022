#include "changeconfirm.h"
#include "ui_changeconfirm.h"

ChangeConfirm::ChangeConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeConfirm)
{
    ui->setupUi(this);
}

ChangeConfirm::~ChangeConfirm()
{
    delete ui;
}
