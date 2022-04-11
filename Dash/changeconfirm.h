#ifndef CHANGECONFIRM_H
#define CHANGECONFIRM_H

#include <QDialog>
#include <QDomElement>
#include <QCanBusFrame>
#include <QTimer>
#include "guicomponent.h"
#include "canhandler.h"

namespace Ui {
class ChangeConfirm;
}

class ChangeConfirm : public QDialog, public GUIComponent
{
    Q_OBJECT

public:
    explicit ChangeConfirm(QWidget *parent = nullptr);
    ~ChangeConfirm();
    void toConfirm(QDomElement const &data, QString const &value);
    void navigate(buttonStates navigation) override;
    void raiseError(QString const &errorMessage) override;

private:
    Ui::ChangeConfirm *ui;
    QDomElement data;
    QString value;
};

#endif // CHANGECONFIRM_H
