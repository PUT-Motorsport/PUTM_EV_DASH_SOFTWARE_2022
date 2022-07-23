#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      subwindowShown(nullptr),
      guiHandler(new GUIHandler()),
      canThread(),
      elapsedTimers(),
      sectorTimes({}),
      currentSector(0),
      updateTimer(new QTimer()),
      ui(new Ui::MainWindow),
      dvSelect(new DvSelect()),
      serviceMode(new ServiceMode()),
      errorCounter(0) {
  ui->setupUi(this);

  timerLabels = {ui->sector1, ui->sector2, ui->sector3};
  guiHandler->moveToThread(&canThread);
  canThread.start(QThread::HighPriority);

  if (canHandler.connected())
    ui->can->setText("CAN Connected");
  else
    ui->can->setText("CAN error");

  // communication between threads
  auto success = QObject::connect(guiHandler, &GUIHandler::updateData, this,
                                  &MainWindow::updateData);
  QObject::connect(guiHandler, &GUIHandler::error, this,
                   &MainWindow::raiseError);
  QObject::connect(guiHandler, &GUIHandler::navigate, this,
                   &MainWindow::navigate);
  QObject::connect(guiHandler, &GUIHandler::setPreset, this,
                   &MainWindow::setPreset);
  QObject::connect(guiHandler, &GUIHandler::clearError, this,
                   &MainWindow::clearError);
  QObject::connect(guiHandler, &GUIHandler::lapPassed, this, &MainWindow::pass);

  QObject::connect(dvSelect, &DvSelect::finished, this, &MainWindow::reopen);
  QObject::connect(serviceMode, &ServiceMode::finished, this,
                   &MainWindow::reopen);

  if (not success) {
    qDebug() << "Signal-slot connection failed";
  }

  QObject::connect(updateTimer, &QTimer::timeout, this,
                   [this]() { updateTimers(); });
  updateTimer->start(timerUpdateTime);

  this->setBMSHVState(BMS_HV_states::AIR_opened);
}

MainWindow::~MainWindow() {
  delete ui;
  delete dvSelect;
  delete serviceMode;
  delete updateTimer;
  delete guiHandler;
}

void MainWindow::updateData(Parameter param, float value) {
  qDebug() << "Received a call" << static_cast<int>(param);
  switch (param) {
    case Parameter::Speed:
      ui->speed->setText(QString::number(value));
      break;
    case Parameter::Power:
      ui->powerBar->setValue(value);
      break;
    case Parameter::RPM:
      break;
    case Parameter::CoolantTemp:
      ui->temperature->setText(QString::number(value) +
                               " °C");  // only parameter shown on both
      serviceMode->updateData(param, value);
      break;
    case Parameter::HVSOC:
      ui->hvsoc->setText("HV: " + QString::number(value) + "%");
      break;
    case Parameter::LVSOC:
      ui->lvsoc->setText("LV: " + QString::number(value) + "%");
      break;
    default:
      serviceMode->updateData(param, value);
  }
}

void MainWindow::raiseError(QString const &errorMessage) {
  logger.add(errorMessage);
  errorCounter++;
  if (subwindowShown == nullptr) {
    ui->error->setText(errorMessage);
    QTimer::singleShot(3000, [this]() { ui->error->setText(""); });
  } else {
    subwindowShown->raiseError(errorMessage);
  }
  ui->errorCounter->setText(QString::number(errorCounter) + " error(s)");
}

void MainWindow::navigate(buttonStates navigation) {
  qDebug() << "Received call to navigate";
  if (subwindowShown == nullptr) {
    switch (navigation) {
        //      case buttonStates::button1_4:
        //        QCoreApplication::quit();
        //        break;
      case buttonStates::button2_3:
        subwindowShown = serviceMode;
        this->hide();
        serviceMode->showFullScreen();
        break;
      default:
        return;  // suppresses a warning, no real use
    }
  } else
    subwindowShown->navigate(navigation);
}

void MainWindow::setPreset(Side side, scrollStates scroll) {
  serviceMode->setPreset(side, scroll);
}

void MainWindow::clearError() {
  if (errorCounter) errorCounter--;

  if (errorCounter > 0)
    ui->errorCounter->setText(QString::number(errorCounter) + "error(s)");
  else
    ui->errorCounter->setText("No errors");
}

void MainWindow::pass(uint8_t sector) {
  auto thisSectorTime = elapsedTimers.at(currentSector).get();
  sectorTimes.at(currentSector) = thisSectorTime;

  if (sector == 0) {
    QCanBusFrame lapTime;
    lapTime.setFrameId(0);
    lapTime.setPayload(QByteArray(reinterpret_cast<char *>(&thisSectorTime),
                                  sizeof(thisSectorTime)));

    canHandler.send(lapTime);
  }

  currentSector = sector;  // update the current sector

  elapsedTimers.at(sector).start();
}

void MainWindow::setMaxPower(uint8_t maxPower) {
  ui->powerBar->setMaximum(maxPower);
}

void MainWindow::setBMSHVState(BMS_HV_states state) {
  switch (state) {
    case BMS_HV_states::AIR_closed:
      ui->bms_hv_state->setText("AIR closed");
      break;
    case BMS_HV_states::AIR_opened:
      ui->bms_hv_state->setText("AIR opened");
      break;
    case BMS_HV_states::Precharge:
      ui->bms_hv_state->setText("Precharge");
      break;
    default:
      ui->bms_hv_state->setText("");
      break;
  }
}

void MainWindow::reopen() {
  subwindowShown = nullptr;
  this->showFullScreen();
}

void MainWindow::updateTimers() {
  for (std::size_t iter = 0; iter < 3; ++iter) {
    timerLabels.at(iter)->setText(
        QString::fromStdString(Timer::toStr(sectorTimes.at(iter))));
  }

  auto const thisSectorTime = elapsedTimers.at(currentSector).get();

  timerLabels.at(currentSector)
      ->setText(QString::fromStdString(Timer::toStr(thisSectorTime)));

  if (thisSectorTime <= sectorTimes.at(currentSector) and
      elapsedTimers.at(currentSector).isValid()) {
    timerLabels.at(currentSector)
        ->setStyleSheet(QStringLiteral("color: green;"));

  }

  else if (thisSectorTime > sectorTimes.at(currentSector) and
           elapsedTimers.at(currentSector).isValid()) {
    timerLabels.at(currentSector)->setStyleSheet(QStringLiteral("color: red;"));

  }

  else {
    timerLabels.at(currentSector)
        ->setStyleSheet(QStringLiteral("color: white;"));
  }
}
