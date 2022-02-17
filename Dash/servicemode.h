#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include <QDialog>
#include "vehicle.h"
#include "logger.h"
namespace Ui {
class ServiceMode;
}

class ServiceMode : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceMode(QWidget *parent = nullptr);
    ~ServiceMode();
    void updateData(Parameter param, qreal value);

private:
    Ui::ServiceMode *ui;
};

#endif // SERVICEMODE_H
