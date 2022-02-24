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
    explicit ChangeConfirm(CanHandler * can, QWidget *parent = nullptr);
    ~ChangeConfirm();
    void toConfirm(QDomElement const &data, QString const &value);
    void navigate(Navigation pressed) override;
    void raiseError(QString const &errorMessage, int errorCode = -1) override;

private:
    Ui::ChangeConfirm *ui;
    QDomElement data;
    CanHandler * can;
    QString value;
};

#endif // CHANGECONFIRM_H
