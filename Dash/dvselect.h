#ifndef DVSELECT_H
#define DVSELECT_H

#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDialog>
#include <QDomDocument>
#include <QTimer>

#include "guicomponent.h"

namespace Ui {
class DvSelect;
}

class /*[[deprecated("Dv mode will not be supported")]]*/ DvSelect
    : public QDialog,
      public GUIComponent {
  Q_OBJECT

 public:
  explicit DvSelect(QWidget *parent = nullptr);
  virtual ~DvSelect();

  void navigate(buttonStates navigation) override;
  void raiseError(QString const &errorMessage) override;

 private:
  void toggleMission(int direction);
  void sendCANframe();

  Ui::DvSelect *ui;
  QDomElement missionsFile;
  QStringList missionNames;
  int missionCount;
  int currentMission;
};

#endif  // DVSELECT_H
