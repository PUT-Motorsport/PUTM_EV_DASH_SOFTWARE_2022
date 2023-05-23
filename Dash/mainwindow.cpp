#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      subwindowShown(nullptr),
      guiHandler(new GUIHandler()),
      canThread(),
    ui(new Ui::MainWindow),
    dvSelect(new DvSelect()),
    serviceMode(new ServiceMode()),
    errorCounter(0) {
  ui->setupUi(this);

  guiHandler->moveToThread(&canThread);
  canThread.start(QThread::HighPriority);

    ui->can->setText("CAN Connected");

  // communication between threads
  QObject::connect(guiHandler, &GUIHandler::updateData, this,
                                  &MainWindow::updateData);
  QObject::connect(guiHandler, &GUIHandler::error, this,
                   &MainWindow::raiseError);
  QObject::connect(guiHandler , &GUIHandler::navigate, this,
                   &MainWindow::navigate);
  QObject::connect(guiHandler, &GUIHandler::setPreset, this,
                   &MainWindow::setPreset);
  QObject::connect(guiHandler, &GUIHandler::clearError, this,
                   &MainWindow::clearError);
  QObject::connect(dvSelect, &DvSelect::finished, this, &MainWindow::reopen);
  QObject::connect(serviceMode, &ServiceMode::finished, this,
                   &MainWindow::reopen);

  this->setBMSHVState(BMS_HV_states::AIR_opened);
}

MainWindow::~MainWindow() {
  delete ui;
  delete dvSelect;
  delete serviceMode;
  delete guiHandler;
}

void MainWindow::updateData(Parameter param, float value) {
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
      ui->hvsoc->setText("HV: " + QString::number(((float)value / 100)) + "%");
      if (value < 20.f) {
          ui->hvsoc->setStyleSheet("color: red;");
      } else if (value < 50.f) {
          ui->hvsoc->setStyleSheet("color: yellow;");
      } else {
          ui->hvsoc->setStyleSheet("color: white;");
      }
      break;
    case Parameter::LVSOC:
      ui->lvsoc->setText("LV: " + QString::number(((float)value) / 100) + "%");
      if (value < 20.f) {
          ui->lvsoc->setStyleSheet("color: red;");
      } else if (value < 50.f) {
          ui->lvsoc->setStyleSheet("color: yellow;");
      } else {
          ui->hvsoc->setStyleSheet("color: white;");
      }
      break;
    case Parameter::Laptime:
      ui->sector2->setText(milliseconds_to_string((uint32_t)value));
    default:
      serviceMode->updateData(param, value);
  }
}

void MainWindow::raiseError(QString const &errorMessage) {
  logger.add(errorMessage);
  errorCounter++;
  if (subwindowShown == nullptr) {
    ui->error->setText(errorMessage);
    this->setStyleSheet("background-color: red;");
    QTimer::singleShot(3000, [this]() { ui->error->setText(""); this->setStyleSheet("background-color: red;"); });
  } else {
    subwindowShown->raiseError(errorMessage);
  }
  ui->errorCounter->setText(QString::number(errorCounter) + " error(s)");
}

void MainWindow::navigate(buttonStates navigation) {
  qDebug() << "Received call to navigate";
  if (subwindowShown == nullptr) {
    switch (navigation) {
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

