#include "canraw.h"
#include "ui_canraw.h"

CanRaw::CanRaw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanRaw)
{
    ui->setupUi(this);
}

CanRaw::~CanRaw()
{
    delete ui;
}
