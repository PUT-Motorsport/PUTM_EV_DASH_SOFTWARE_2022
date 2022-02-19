#include "servicemode.h"
#include "ui_servicemode.h"

ServiceMode::ServiceMode(CanHandler * can, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceMode), subwindowShown(nullptr)
{
    ui->setupUi(this);
    driving = new DrivingSelect(can);
    canRaw = new CanRaw();
    logs = new Logs();

    QObject::connect(driving, &DrivingSelect::finished, this, &ServiceMode::reopen);
    QObject::connect(canRaw, &CanRaw::finished, this, &ServiceMode::reopen);
    QObject::connect(logs, &Logs::finished, this, &ServiceMode::reopen);
}

ServiceMode::~ServiceMode()
{
    delete ui;
    delete driving;
    delete canRaw;
    delete logs;
}

void ServiceMode::updateData(Parameter param, qreal value)
{
    QString valueStr = QString::number(value);
    switch(param) {
    case Parameter::APPS:
        ui->apps->setText("APPS Value: " + valueStr + '%');
        break;
    case Parameter::BmshvTemp:
        ui->bmshvtemp->setText("BMSHV: " + valueStr + " °C");
        break;
    case Parameter::BmshvVoltage:
        ui->bmshvvoltage->setText("BMSHV: " + valueStr + " V");
        break;
    case Parameter::BmslvTemp:
        ui->bmslvtemp->setText("BMSLV: " + valueStr + " °C");
        break;
    case Parameter::BmslvVoltage:
        ui->bmslvvoltage->setText("BMSLV: " + valueStr + " V");
        break;
    case Parameter::CoolantTemp:
        ui->coolanttemp->setText("Coolant: " + valueStr + "  °C");
        break;
    case Parameter::EngineTemp:
        ui->enginetemp->setText("Engine: " + valueStr + " °C");
        break;
    case Parameter::InverterTemp:
        ui->invertertemp->setText("Inverter: " + valueStr + " °C");
        break;
    default:
        Logger::add("Service mode received a wrong argument.");
    }
}

void ServiceMode::navigate(Navigation pressed)
{
    if (subwindowShown == nullptr) {
        switch (pressed) {
        case Navigation::X:
            subwindowShown = canRaw;
            this->hide();
            canRaw->show();
            break;
        case Navigation::B:
            this->done(QDialog::Accepted);
            break;
        case Navigation::Y:
            subwindowShown = logs;
            this->hide();
            logs->show();
            break;
        case Navigation::A:
            subwindowShown = driving;
            this->hide();
            driving->show();
            break;
        default:
            return;     //suppresses a warning, no real use
        }
    }
    else
        subwindowShown->navigate(pressed);
}

void ServiceMode::raiseError(int errorCode, const QString &errorMessage)
{
    if (subwindowShown == nullptr) {
        ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    }
    else
        subwindowShown->raiseError(errorCode, errorMessage);
}

void ServiceMode::reopen()
{
    subwindowShown = nullptr;
    this->show();
}
