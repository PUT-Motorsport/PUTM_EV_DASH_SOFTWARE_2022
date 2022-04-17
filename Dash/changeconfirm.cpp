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

void ChangeConfirm::toConfirm(Side side, scrollStates state)
{
    currentSide = side;

    //prepare the window

    ui->setting->setText(settingNames[currentSide]);

    ui->newValue->setText(QString::number(settingValues.at(currentSide).at(currentValueIndex[currentSide])));

}

void ChangeConfirm::navigate(buttonStates navigation)
{
    if(navigation not_eq buttonStates::button1) {

        this->done(QDialog::Rejected);
        return;
    }

//    auto frameToSend = ???

}

void ChangeConfirm::raiseError(const QString &errorMessage)
{
    ui->error->setText("Error: " + errorMessage);

    QTimer::singleShot(2000, [this] () {
        ui->error->setText("");
    });
}
