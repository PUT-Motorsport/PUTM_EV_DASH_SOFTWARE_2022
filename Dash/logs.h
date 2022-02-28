#ifndef LOGS_H
#define LOGS_H

#include <QDialog>
#include <QTimer>

#include "guicomponent.h"

namespace Ui {
class Logs;
}

class Logs : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr);       //TODO: find a way to load the log files instead of sniffing them
    ~Logs();
    void navigate(Navigation pressed) override;
    void raiseError(QString const &errorMessage, int errorCode = -1) override;

    void startSniffing();
private:
    void sniff();
    Ui::Logs *ui;
    QString newLine;
    QTimer * refreshTimer;
    static constexpr int refreshTime = 100;
    static constexpr unsigned short int maxLines = 20;
    unsigned short int currentLines;
};

#endif // LOGS_H
