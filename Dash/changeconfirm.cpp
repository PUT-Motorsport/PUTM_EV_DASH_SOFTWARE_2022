#include "changeconfirm.h"
#include "ui_changeconfirm.h"

ChangeConfirm::ChangeConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeConfirm)
{
    ui->setupUi(this);
}

ChangeConfirm::~ChangeConfirm()
{
    delete ui;
}

void ChangeConfirm::toConfirm(const QDomElement &confirmData, const QString &newValue)
{
    ui->setting->setText(confirmData.attribute("description"));
    ui->newValue->setText(newValue);

    data = confirmData;
    value = newValue;
}

void ChangeConfirm::navigate(buttonStates navigation)
{
    if (navigation not_eq buttonStates::button1) {
        this->done(QDialog::Accepted);
        return;
    }
    QCanBusFrame frame;
    assert(data.attribute("sendID") not_eq "");
    assert(data.attributes().length() > 0);
    bool conversion = false;
    frame.setFrameId(data.attribute("sendID").toInt(&conversion, 16));
    if (not(conversion)) {
        raiseError("New frame parsing failed");
        logger.add("Outgoing frame id parsing failed", LogType::Critical);
    }
    QString payloadStr = QString::number(value.toInt(), 16);
    frame.setPayload(QByteArray::fromHex(payloadStr.toUtf8()));
    frame.setFrameType(QCanBusFrame::FrameType::DataFrame);

    if (canHandler.send(frame))
        this->done(QDialog::Accepted);
    else {
        raiseError("NOT SENT");
        return;
    }

    //if succesful, clear the screen
    ui->setting->setText("");
    ui->newValue->setText("");
}

void ChangeConfirm::raiseError(const QString &errorMessage)
{
    ui->error->setText("Error: " + errorMessage);

    QTimer::singleShot(2000, [this] () {
        ui->error->setText("");
    });
}
