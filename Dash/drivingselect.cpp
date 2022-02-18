#include "drivingselect.h"
#include "ui_drivingselect.h"

DrivingSelect::DrivingSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrivingSelect), current(Setting::Regain)
{
    ui->setupUi(this);
    ui->regain->setStyleSheet("color: red;");
}

DrivingSelect::~DrivingSelect()
{
    delete ui;
}

void DrivingSelect::navigate(Navigation pressed)
{
    switch (pressed) {
    case Navigation::X:
        changeHighlight();
        break;
    default:
        return; //TODO: rest of the buttons
    }
}

void DrivingSelect::raiseError(int errorCode, const QString &errorMessage)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
}

void DrivingSelect::keyPressEvent(QKeyEvent *event)
{
    changeHighlight();
}

void DrivingSelect::changeHighlight()
{

    if (current == Setting::Apps)
        current = Setting::Regain;
    else
        current = static_cast<Setting>(static_cast<int>(current) + 1);

    QString white = "color: white;";
    QString red = "color: red;";
    ui->power->setStyleSheet(white);
    ui->apps->setStyleSheet(white);
    ui->regain->setStyleSheet(white);
    ui->fan->setStyleSheet(white);
    ui->traction->setStyleSheet(white);

    switch (current) {
    case Setting::Apps:
        ui->apps->setStyleSheet(red);
        break;
    case Setting::Fan:
        ui->fan->setStyleSheet(red);
        break;
    case Setting::Traction:
        ui->traction->setStyleSheet(red);
        break;
    case Setting::Power:
        ui->power->setStyleSheet(red);
        break;
    case Setting::Regain:
        ui->regain->setStyleSheet(red);
        break;
    default:
        Logger::add("changeHighlight received a wrong argument");
    }
}
