#ifndef LOGS_H
#define LOGS_H

#include <QDialog>
#include <QTimer>

#include "guicomponent.h"

namespace Ui {
class Logs;
}

class Logs : public QDialog, public GUIComponent {
  Q_OBJECT
  // should this class be a singleton?
 public:
  explicit Logs(QWidget *parent =
                    nullptr);  // TODO: find a way to load the log files instead
                               // of sniffing them (probably not possible,
                               // thanks to a journaling filesystem)
  ~Logs();
  void navigate(buttonStates navigation) override;
  void raiseError(QString const &errorMessage) override;

  void startSniffing();

 private:
  void sniff();
  Ui::Logs *ui;
  QString newLine;
  QTimer *refreshTimer;
  static constexpr int refreshTime = 100;
  static constexpr uint8_t maxLines = 20;
  uint8_t currentLines;
};

#endif  // LOGS_H
