#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , subwindowShown(nullptr), interruptSubwindowShown(nullptr), m_speed(0),
      timerStarted(false), ui(new Ui::MainWindow), errorCounter(0)
{
    ui->setupUi(this);

    if (canHandler.connected())
        ui->can->setText("CAN Connected");
    else
        ui->can->setText("CAN error");

    dvSelect = new DvSelect();
    serviceMode = new ServiceMode();
    changeConfirm = new ChangeConfirm();

    QObject::connect(dvSelect, &DvSelect::finished, this, &MainWindow::reopen);
    QObject::connect(serviceMode, &ServiceMode::finished, this, &MainWindow::reopen);
    QObject::connect(changeConfirm, &ChangeConfirm::finished, this, &MainWindow::reopen);

    elapsedTimer = new QElapsedTimer();

    QObject::connect(&updateTimer, &QTimer::timeout, this, [=](){
        QTime time = QTime::fromMSecsSinceStartOfDay(elapsedTimer->elapsed());
        ui->currentTime->setText(time.toString("hh:mm:ss:zzz"));
    });
    best.setHMS(0,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dvSelect;
    delete serviceMode;
    delete elapsedTimer;
    delete changeConfirm;
}

void MainWindow::updateData(Parameter param, qreal value)
{
    switch (param) {
    case Parameter::Speed:
        ui->speed->setText(QString::number(value));
        if (m_speed == 0 and value != 0 and not(timerStarted)) {
            elapsedTimer->start();
            updateTimer.start(50);
            timerStarted = true;
        }
        m_speed = value;
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

void MainWindow::raiseError(QString const &errorMessage, int errorCode)
{
    if (subwindowShown == nullptr) {
        ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
        errorCounter++;
        QTimer::singleShot(3000, [this] () {
                ui->error->setText("");
            });
    }
    else {
        subwindowShown->raiseError(errorMessage, errorCode);
    }
    ui->errorCounter->setText(QString::number(errorCounter) + " errorr(s)");
}

void MainWindow::navigate(Navigation pressed)
{
    if (subwindowShown == nullptr) {
        switch (pressed) {
        case Navigation::A:
            subwindowShown = dvSelect;
            this->hide();
            dvSelect->show();
            break;
        case Navigation::B:
            subwindowShown = serviceMode;
            this->hide();
            serviceMode->show();
            break;
        case Navigation::X:
            updateBestTime();
            break;
        case Navigation::Y:
            resetTimer();
            break;
        default:
            return;     //suppresses a warning, no real use
        }
    }
    else subwindowShown->navigate(pressed);
}

void MainWindow::getConfirmation(const QDomElement &data, QString value)
{
    interruptSubwindowShown = subwindowShown;
    subwindowShown = changeConfirm;
    changeConfirm->toConfirm(data, value);      //prepare the subwindow
    this->hide();
    changeConfirm->show();
}

void MainWindow::reopen()
{
    if (interruptSubwindowShown == nullptr) {   //if changeconfirm didn't trigger the change
        this->show();
        subwindowShown = nullptr;
    }

    interruptSubwindowShown = nullptr;
}

void MainWindow::updateBestTime()
{
    QTime time = QTime::fromMSecsSinceStartOfDay(elapsedTimer->elapsed());
    logger.add("Finished a lap in " + time.toString("hh:mm:ss:zzz"));
    if (best == QTime(0,0,0) or best > time) {
        best = time;
    }
    else {
        elapsedTimer->restart();
        return;
    }
    ui->bestTime->setText(time.toString("hh:mm:ss:zzz"));
    elapsedTimer->restart();
}

void MainWindow::resetTimer()
{
    if (m_speed != 0)
        elapsedTimer->restart();
    else {
        updateTimer.stop();
        delete elapsedTimer;
        elapsedTimer = new QElapsedTimer();
        timerStarted = false;
    }
    ui->currentTime->setText(QTime(0, 0, 0).toString("hh:mm:ss:zzz"));
    ui->bestTime->setText(QTime(0, 0, 0).toString("hh:mm:ss:zzz"));
}





