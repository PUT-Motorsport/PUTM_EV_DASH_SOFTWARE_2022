#include "servicemode.h"
#include "ui_servicemode.h"

ServiceMode::ServiceMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceMode)
{
    ui->setupUi(this);
}

ServiceMode::~ServiceMode()
{
    delete ui;
}

void ServiceMode::update(Parameter param, qreal value)
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
