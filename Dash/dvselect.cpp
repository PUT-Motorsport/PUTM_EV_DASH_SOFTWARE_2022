#include "dvselect.h"
#include "ui_dvselect.h"

DvSelect::DvSelect(CanHandler * can, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DvSelect), missionCount(0), currentMission(0), can(can)
{
    ui->setupUi(this);
    Logger::loadXML(missionsFile, pathToXML);
    //load mission names
    QDomNode iterator = missionsFile.firstChild();
    while (not(iterator.isNull())) {
        QDomElement element = iterator.toElement();
        if (element.attribute("description") != "") {
            missionNames.append(element.attribute("description"));
            missionCount++;
        }
        iterator = iterator.nextSibling();
   }
   if (missionNames.length() != 0)
       ui->mission->setText(missionNames.at(0));    //display the first mission
}

DvSelect::~DvSelect()
{
    delete ui;
}

void DvSelect::navigate(Navigation pressed)
{
    switch (pressed) {
    case Navigation::B:
        this->done(QDialog::Accepted);
        break;
    case Navigation::A:
        sendCANframe();
        break;
    case Navigation::X:
        toggleMission(-1);
        break;
    case Navigation::Y:
        toggleMission(1);
        break;
    default:
        return;  //warning suppression
    }
}

void DvSelect::raiseError(int errorCode, const QString &errorMessage)
{
    ui->error->setText("Error " + QString::number(errorCode) + ": " + errorMessage);
    QTimer::singleShot(3000, [this] () {
            ui->error->setText("");
        });
}

void DvSelect::toggleMission(int direction)
{
    if (missionNames.length() == 0) {
        raiseError(1, "File missing");
        return;
    }
    currentMission += direction;
    if (currentMission >= missionCount)
        currentMission = 0;
    if (currentMission < 0)
        currentMission = missionCount - 1;
    ui->mission->setText(missionNames.at(currentMission));
}

void DvSelect::sendCANframe()
{
    QDomNode iterator = missionsFile.firstChild();
    for (int i = 0; i < currentMission; i++) {
        iterator = iterator.nextSibling();      //move the document tree branch to the correct mission
    }
    if (iterator.isNull()) {
        raiseError(1, "File Error");
        Logger::add("File error while sending dv can frame", LogType::Error);
        return;
    }
    QDomElement element = iterator.toElement();
    int frameID;
    if (element.attribute("frameID") != "")
        frameID = element.attribute("frameID").toInt();
    else {
        raiseError(1, "Can Sender Error");  //inform the user live
        Logger::add("Can dv frame couldn't be sent", LogType::Error);
        return;
    }
    QString payloadStr = element.attribute("framePayload");
    QCanBusFrame canFrame;
    canFrame.setFrameId(frameID);
    canFrame.setPayload(QByteArray::fromHex(payloadStr.toUtf8()));
    canFrame.setFrameType(QCanBusFrame::FrameType::DataFrame);

    if (can->send(canFrame)) {
        Logger::add("Sent dv can frame " + element.attribute("description"));
        ui->sent->setText("Sent!");
        QTimer::singleShot(1000, [this] () {
                ui->sent->setText("");
            });
    }
    else {
        ui->sent->setText("NOT Sent!");
        QTimer::singleShot(1000, [this] () {
                ui->sent->setText("");
            });
    }
}
