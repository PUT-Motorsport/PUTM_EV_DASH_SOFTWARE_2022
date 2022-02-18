#ifndef LOGS_H
#define LOGS_H

#include <QDialog>

#include "guicomponent.h"

namespace Ui {
class Logs;
}

class Logs : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr);
    ~Logs();
    void navigate(Navigation pressed) override;
    void raiseError(int errorCode, QString const &errorMessage) override;

private:
    Ui::Logs *ui;
    QString const pathToLogs = QStringLiteral("");  //TODO
};

#endif // LOGS_H
