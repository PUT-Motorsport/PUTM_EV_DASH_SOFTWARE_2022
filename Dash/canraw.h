#ifndef CANRAW_H
#define CANRAW_H

#include <QDialog>

namespace Ui {
class CanRaw;
}

class CanRaw : public QDialog
{
    Q_OBJECT

public:
    explicit CanRaw(QWidget *parent = nullptr);
    ~CanRaw();

private:
    Ui::CanRaw *ui;
};

#endif // CANRAW_H
