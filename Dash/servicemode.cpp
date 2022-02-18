#include "servicemode.h"
#include "ui_servicemode.h"

ServiceMode::ServiceMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceMode), subwindowShown(nullptr)
{
    ui->setupUi(this);
    driving = new DrivingSelect();
    canRaw = new CanRaw();
    logs = new Logs();
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
    switch (pressed) {
        case Navigation::A:
        subwindowShown = canRaw;
        canRaw->exec();
        subwindowShown = nullptr;
        break;
    case Navigation::B:
        this->done(QDialog::Accepted);
        break;
    case Navigation::X:
        subwindowShown = logs;
        logs->exec();
        subwindowShown = nullptr;
        break;
    case Navigation::Y:
        subwindowShown = driving;
        driving->exec();
        subwindowShown = nullptr;
        break;
    default:
        return;     //suppresses a warning, no real use
    }
}

void ServiceMode::raiseError(int errorCode, const QString &errorMessage)
{
    if (subwindowShown == nullptr) {
        ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    }
    else
        subwindowShown->raiseError(errorCode, errorMessage);
}
