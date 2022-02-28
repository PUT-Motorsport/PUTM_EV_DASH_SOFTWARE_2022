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
        logger.add("Service mode received a wrong argument.");
    }
}

void ServiceMode::navigate(Navigation pressed)
{
    if (subwindowShown == nullptr) {
        switch (pressed) {
        case Navigation::X:
            subwindowShown = canRaw;
            this->hide();
            canRaw->startSniffing();
            canRaw->show();
            break;
        case Navigation::B:
            this->done(QDialog::Accepted);
            break;
        case Navigation::Y:
            subwindowShown = logs;
            this->hide();
            logs->startSniffing();
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

void ServiceMode::raiseError(const QString &errorMessage, int errorCode)
{
    if (subwindowShown == nullptr) {
        ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
        QTimer::singleShot(3000, [this] () {
                ui->error->setText("");
            });
    }
    else
        subwindowShown->raiseError(errorMessage, errorCode);
}

void ServiceMode::reopen()
{
    subwindowShown = nullptr;
    this->show();
}
