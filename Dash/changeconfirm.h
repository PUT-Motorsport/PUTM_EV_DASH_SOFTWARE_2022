#ifndef CHANGECONFIRM_H
#define CHANGECONFIRM_H

#include <QDialog>

namespace Ui {
class ChangeConfirm;
}

class ChangeConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeConfirm(QWidget *parent = nullptr);
    ~ChangeConfirm();

private:
    Ui::ChangeConfirm *ui;
};

#endif // CHANGECONFIRM_H
