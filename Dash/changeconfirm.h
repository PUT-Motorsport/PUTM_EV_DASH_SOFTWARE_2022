#ifndef CHANGECONFIRM_H
#define CHANGECONFIRM_H

#include <QDialog>
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
    void toConfirm(Side side, scrollStates state);
    void navigate(buttonStates navigation) override;
    void raiseError(QString const &errorMessage) override;

private:
    Ui::ChangeConfirm *ui;

    static constexpr std::array<std::array<uint8_t, 4>, 2>  settingValues{{{1, 2, 3, 4}, {5, 6, 7, 8}}};

    uint8_t currentValueIndex[2];

    Side currentSide;

    QString const settingNames[2]{"Brakes", "Energy Regain"};    //0-left 1-right

};

#endif // CHANGECONFIRM_H
