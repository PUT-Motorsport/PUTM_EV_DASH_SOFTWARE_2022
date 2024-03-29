#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "canhandler.h"
#include "dvselect.h"
#include "guicomponent.h"
#include "guihandler.h"
#include "servicemode.h"
#include "timer.h"
#include "vehicle.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  GUIComponent
      *subwindowShown;  // ensures two-way communication between classes

 public slots:

  void updateData(Parameter param, float value);
  void raiseError(QString const &errorMessage);
  void navigate(buttonStates navigation);
  void setPreset(Side side, scrollStates scroll);
  void clearError();
  void setMaxPower(uint8_t maxPower);
  void setBMSHVState(BMS_HV_states state);

 private slots:
  void reopen();

 private:
  GUIHandler *guiHandler;
  QThread canThread;

  void updateBestTime();
  void resetTimer();

  static constexpr uint8_t delta = 2;
  static constexpr uint16_t timerUpdateTime = 100;

  Ui::MainWindow *ui;

  DvSelect *dvSelect;
  ServiceMode *serviceMode;

  uint8_t errorCounter;
};
#endif  // MAINWINDOW_H
