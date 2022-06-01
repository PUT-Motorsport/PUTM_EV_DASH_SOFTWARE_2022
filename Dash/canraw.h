#ifndef CANRAW_H
#define CANRAW_H

#include "guicomponent.h"
#include <QDialog>
#include <QTimer>

namespace Ui {
class CanRaw;
}

class CanRaw : public QDialog, public GUIComponent {
  Q_OBJECT

public:
  explicit CanRaw(QWidget *parent = nullptr);
  ~CanRaw();
  void navigate(buttonStates navigation) override;
  void raiseError(QString const &errorMessage) override;

  void startSniffing();

private:
  void sniff();
  QTimer *refreshTimer;
  QString newLine;
  static constexpr int refreshTime = 300;

  static constexpr int maxLines = 25;
  unsigned short int currentLines;
  Ui::CanRaw *ui;
};

#endif // CANRAW_H
