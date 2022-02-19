#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , subwindowShown(nullptr), ui(new Ui::MainWindow),
      canStatus(Status::Unresolved), errorCounter(0)
{
    ui->setupUi(this);
    canHandler = new CanHandler();
    if (canHandler->connect())
        ui->can->setText("CAN Connected");
    else
        ui->can->setText("CAN error");

    QObject::connect(canHandler, &CanHandler::raiseError, this, &MainWindow::raiseError);
    QObject::connect(canHandler, &CanHandler::updateGUI, this, &MainWindow::updateData);
    QObject::connect(canHandler, &CanHandler::navigation, this, &MainWindow::navigate);


    dvSelect = new DvSelect(canHandler);        //needs to send frames
    serviceMode = new ServiceMode(canHandler);

    QObject::connect(dvSelect, &DvSelect::finished, this, &MainWindow::reopen);
    QObject::connect(serviceMode, &ServiceMode::finished, this, &MainWindow::reopen);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canHandler;
    delete dvSelect;
    delete serviceMode;
}

void MainWindow::updateData(Parameter param, qreal value)
{
    switch (param) {
    case Parameter::Speed:
        ui->speed->setText(QString::number(value));
        break;
    case Parameter::Power:
        ui->power->setText("Power: " + QString::number(value) + " kW");
        break;
    case Parameter::RPM:
        ui->rpmBar->setValue(value);
        break;
    case Parameter::CoolantTemp:
        ui->temperature->setText("Temp: " + QString::number(value) + " °C");    //only parameter shown on both
        serviceMode->updateData(param, value);
        break;
    case Parameter::SOC:
        ui->soc->setText("SOC: " + QString::number(value));
        break;
    default:
        serviceMode->updateData(param, value);
    }
}

void MainWindow::raiseError(int errorCode, QString const &errorMessage)
{
    if (subwindowShown == nullptr) {
        ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
        errorCounter++;
    }
    else {
        subwindowShown->raiseError(errorCode, errorMessage);
    }
}

void MainWindow::navigate(Navigation pressed)
{
    if (subwindowShown == nullptr) {
        switch (pressed) {
        case Navigation::A:
            subwindowShown = dvSelect;  //TODO: A function to reset the subwindow shown
            this->hide();
            dvSelect->show();
            break;
        case Navigation::B:
            subwindowShown = serviceMode;
            this->hide();
            serviceMode->show();
            break;
        default:
            return;     //suppresses a warning, no real use
        }
    }
    else subwindowShown->navigate(pressed);
}

void MainWindow::reopen()
{
    subwindowShown = nullptr;
    this->show();
}





