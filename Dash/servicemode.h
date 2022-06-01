#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include <QDialog>
#include <QTimer>

#include "canraw.h"
#include "drivingselect.h"
#include "guicomponent.h"
#include "logs.h"

#include "canhandler.h"
#include "logger.h"
#include "vehicle.h"

namespace Ui {
class ServiceMode;
}

class ServiceMode : public QDialog, public GUIComponent {
  Q_OBJECT

public:
  explicit ServiceMode(QWidget *parent = nullptr);
  ~ServiceMode();
  void updateData(Parameter param, int value);
  void navigate(buttonStates navigation) override;
  void raiseError(QString const &errorMessage) override;
  void setPreset(Side side, scrollStates scroll) {
    driving->setPreset(side, scroll);
  }

private:
  Ui::ServiceMode *ui;
  DrivingSelect *driving;
  Logs *logs;
  CanRaw *canRaw;
  GUIComponent *subwindowShown;
private slots:
  void reopen();
};

#endif // SERVICEMODE_H
