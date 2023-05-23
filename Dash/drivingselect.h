#ifndef DRIVINGSELECT_H
#define DRIVINGSELECT_H

#include <QCanBusFrame>
#include <QDialog>
#include <QLabel>
#include <QRegularExpression>
#include <QTimer>

#include "guicomponent.h"
#include "lib/CRC.h"

namespace Ui {
class DrivingSelect;
}

class DrivingSelect : public QDialog, public GUIComponent {
  Q_OBJECT

 public:
  explicit DrivingSelect(QWidget *parent = nullptr);
  ~DrivingSelect();
  void navigate(buttonStates navigation) override;
  void raiseError(QString const &errorMessage) override;

  void setPreset(Side side, scrollStates scroll);

 private:
  void sendSettings();
  void changeHighlight();
  void resetToCurrent();
  uint8_t calculateCRC();
  uint8_t getSettingValue(Setting setting, bool next);
  void changeSettingValue(bool next);

  Setting currentSetting;  // todo: rename
  Dash_TCS previousSettings;
  Dash_TCS currentSettings;

  Ui::DrivingSelect *ui;
  QList<QPair<QLabel *, QLabel *>> labels;

  QString const getSettingName(Setting setting, uint8_t value);

  QList<QString> presets;
  QString const presetsFileName{"presets.csv"};
  QList<QString> descriptions;
  QString const descriptionsFileName{"names.txt"};
  QList<QString> values;
  QString const valuesFileName{"settings.csv"};

  QList<QString> loadFile(QString const &fileName);

  // crc
  CRC::Table<uint8_t, sizeof(uint8_t)> table;
};

#endif  // DRIVINGSELECT_H
