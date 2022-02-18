#ifndef CANRAW_H
#define CANRAW_H

#include <QDialog>

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
    void raiseError(int errorCode, QString const &errorMessage) override;

private:
    Ui::CanRaw *ui;
};

#endif // CANRAW_H
