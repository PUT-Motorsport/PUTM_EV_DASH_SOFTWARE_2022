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

void CanRaw::navigate(Navigation pressed)
{
    this->done(QDialog::Accepted);
}

void CanRaw::raiseError(QString const &errorMessage, int errorCode)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
        });
}
