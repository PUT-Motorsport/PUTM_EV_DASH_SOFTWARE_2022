#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "guicomponent.h"
#include "guihandler.h"

#include "canhandler.h"
#include "dvselect.h"
#include "servicemode.h"
#include "timer.h"
#include "vehicle.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
// remember to zero all values on screens
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  GUIComponent *subwindowShown; // ensures two-way communication between classes
  GUIComponent
      *interruptSubwindowShown; // allows to store previously open window if
                                // change confirmation "interrupts"

public slots:

  void updateData(Parameter param, float value);
  void raiseError(QString const &errorMessage);
  void navigate(buttonStates navigation);
  void setPreset(Side side, scrollStates scroll);
  void clearError();
  void pass(uint8_t sector);
  void setMaxPower(uint8_t maxPower);
  void setBMSHVState(BMS_HV_states state);
private slots:
  void reopen();

private:
  GUIHandler *guiHandler;
  QThread canThread;

  void updateBestTime();
  void resetTimer();

  std::array<QLabel *, 3> timerLabels;
  std::array<Timer, 3> elapsedTimers;
  std::array<std::chrono::duration<long, std::ratio<1, 1000>>::rep, 3>
      sectorTimes;
  uint8_t currentSector;
  static constexpr uint8_t delta = 2;
  static constexpr uint16_t timerUpdateTime = 100;
  void updateTimers();
  QTimer *updateTimer; // will call the gui to update timer

  Ui::MainWindow *ui;

  DvSelect *dvSelect;
  ServiceMode *serviceMode;

  uint8_t errorCounter;
};
#endif // MAINWINDOW_H
