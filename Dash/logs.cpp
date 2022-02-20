#include "logs.h"
#include "ui_logs.h"

Logs::Logs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);
}

Logs::~Logs()
{
    delete ui;
}

void Logs::navigate(Navigation pressed)
{
    this->done(QDialog::Accepted);
}

void Logs::raiseError(int errorCode, const QString &errorMessage)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
        });
}
