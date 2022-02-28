#ifndef CANRAW_H
#define CANRAW_H

#include <QDialog>
#include <QTimer>
#include "guicomponent.h"

namespace Ui {
class CanRaw;
}

class CanRaw : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit CanRaw(QWidget *parent = nullptr);
    ~CanRaw();
    void navigate(Navigation pressed) override;
    void raiseError(QString const &errorMessage, int errorCode = -1) override;

    void startSniffing();
private:
    void sniff();
    QTimer * refreshTimer;
    QString newLine;
    static constexpr int refreshTime = 300;

    static constexpr int maxLines = 25;
    unsigned short int currentLines;
    Ui::CanRaw *ui;
};

#endif // CANRAW_H
