#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) , subwindowShown(nullptr), interruptSubwindowShown(nullptr), guiHandler(new GUIHandler()),
      canThread(), m_speed(0), timerStarted(false),  updateTimer(new QTimer()), ui(new Ui::MainWindow), errorCounter(0)
{
    ui->setupUi(this);

    guiHandler->moveToThread(&canThread);
    canThread.start(QThread::InheritPriority);

    if (canHandler.connected())
        ui->can->setText("CAN Connected");
    else
        ui->can->setText("CAN error");

    dvSelect = new DvSelect();
    serviceMode = new ServiceMode();
    changeConfirm = new ChangeConfirm();

    //communication between threads
    QObject::connect(guiHandler, &GUIHandler::updateData, this, &MainWindow::updateData);
    QObject::connect(guiHandler, &GUIHandler::error, this, &MainWindow::raiseError);
    QObject::connect(guiHandler, &GUIHandler::navigate, this, &MainWindow::navigate);
    QObject::connect(guiHandler, &GUIHandler::getConfirmation, this, &MainWindow::getConfirmation);
    QObject::connect(guiHandler, &GUIHandler::clearError, this, &MainWindow::clearError);

    QObject::connect(dvSelect, &DvSelect::finished, this, &MainWindow::reopen);
    QObject::connect(serviceMode, &ServiceMode::finished, this, &MainWindow::reopen);
    QObject::connect(changeConfirm, &ChangeConfirm::finished, this, &MainWindow::reopen);

    elapsedTimer = new QElapsedTimer();

    QObject::connect(updateTimer, &QTimer::timeout, this, [=](){
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
    delete updateTimer;
    delete guiHandler;
}

void MainWindow::updateData(Parameter param, qreal value)
{
    switch (param) {
    case Parameter::Speed:
        ui->speed->setText(QString::number(value));
        if (m_speed == 0 and value != 0 and not(timerStarted)) {
            elapsedTimer->start();
            updateTimer->start(50);
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

void MainWindow::raiseError(QString const &errorMessage)
{
    logger.add(errorMessage);
    errorCounter++;
    if (subwindowShown == nullptr) {
        ui->error->setText(errorMessage);
        QTimer::singleShot(3000, [this] () {
                ui->error->setText("");
            });
    }
    else {
        subwindowShown->raiseError(errorMessage);
    }
    ui->errorCounter->setText(QString::number(errorCounter) + " error(s)");
}

void MainWindow::navigate(buttonStates navigation)
{
    if (subwindowShown == nullptr) {
        switch (navigation) {
        case buttonStates::button1_4:
            subwindowShown = dvSelect;
            this->hide();
            dvSelect->show();
            break;
        case buttonStates::button2_3:
            subwindowShown = serviceMode;
            this->hide();
            serviceMode->show();
            break;
        case buttonStates::button1:
            updateBestTime();
            break;
        case buttonStates::button2:
            resetTimer();
            break;
        default:
            return;     //suppresses a warning, no real use
        }
    }
    else subwindowShown->navigate(navigation);
}

void MainWindow::getConfirmation(Side side, scrollStates scroll)
{
    interruptSubwindowShown = subwindowShown;   //keeps the window that change confirm interrupted
    subwindowShown = changeConfirm;
    changeConfirm->toConfirm(side, scroll);      //prepare the subwindow
    this->hide();
    changeConfirm->show();
}

void MainWindow::clearError()
{
    if (errorCounter)
        errorCounter--;

    if (errorCounter > 0)
        ui->errorCounter->setText(QString::number(errorCounter) + "error(s)");
    else
        ui->errorCounter->setText("No errors");
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
        updateTimer->stop();
        delete elapsedTimer;
        elapsedTimer = new QElapsedTimer();
        timerStarted = false;
    }
    ui->currentTime->setText(QTime(0, 0, 0).toString("hh:mm:ss:zzz"));
    ui->bestTime->setText(QTime(0, 0, 0).toString("hh:mm:ss:zzz"));
}





